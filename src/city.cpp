#include "inc/city.hpp"

#include <chrono>
#include <functional>

city::city(size_t height, size_t weight)
    : _height(height), _weight(weight), _car_no(0), _flow_stat(STOP), _dt(1), _delay_milliseconds(20)
{
    this->_blocks = new joint_ptr*[this->_height];
    // init joints
    FOR(i,0,this->_height,++) this->_blocks[i] = new joint_ptr[this->_weight];
    FOR(i,0,this->_height,++)
        FOR(j,0,this->_weight,++)
            this->_blocks[i][j] = joint_ptr(new joint("J["+std::to_string(i)+","+std::to_string(j)+"]"));
    // adding streets to joints
    // topology:
    //    +--+--+
    //    |  |  |
    //    +--+--+
    //    |  |  |
    //    +--+--+
    // joint's street indexing:
    //    RIGHT: 0
    //    DOWN:  1
    //    LEFT:  2
    //    UP:    3
    vector<street_ptr> streets;
    FOR(i,0,this->_height,++) {
        FOR(j,0,this->_weight,++) {
            // @ every instance at max create 2 streets [ RIGHT / DOWN ]
            //  > the last row only creates [ RIGHT ] street
            //  > the last column only creates [ DOWN ] street
            //  > the last(bottom right) joint don't creates anything!
            //  > streets are joint by their HEADs while creating
            // @ every joint at max uses the previous [ LEFT / UP ] joint's streets
            //  > the first row only uses the [ LEFT ] joint's [ RIGHT ] street
            //  > the first column only uses the [ UP ] joint's [ DOWN ] street
            //  > the first(top left) joint don't uses anything!
            //  > streets are joint by their TAILs while using
            // for not available branches `nullptr` will be assigned instead
            bool
                is_first_row   = (i == 0),
                is_last_row    = ((i + 1) == this->_height),
                is_first_col   = (j == 0),
                is_last_col    = ((j + 1) == this->_weight);
            // fetch the joint
            joint_ptr& cj = this->_blocks[i][j];
            // add outgoing street to joint
            {
                // flags the condition that we can create a street
                bool flags[2] = { !is_last_col, !is_last_row };
                string postfix[2] = { "-R", "-D" };
                FOR(k,0,2,++) {
                    if(flags[k]) {
                        // random capacity between [ 1km, 8km ]
                        size_t cap = ((1000 / CONST_AVG_CAR_LONG) + rand() % int(7000 / CONST_AVG_CAR_LONG));
                        street_ptr s  = street_ptr(new street(cap, "S-"+cj->name() + postfix[k]));
                        s->event_add()
                            (street::AFTER_EXIT, [](vector<const void*> args) {
                                assert(args.size() == 3);
                                const car* c = reinterpret_cast<const car*>(args[0]);
                                const street
                                    *s = reinterpret_cast<const street*>(args[1]),
                                    *t = reinterpret_cast<const street*>(args[2]);
                                // making it thread safe
                                stringstream ss;
                                ss<<"$ Car# «" << c->getID() <<"» going to «" << ::to_string(c->direction()) << "» with max speed of «"<<c->max_speed()<<" m/s» exited from «" << s->name() << " @ " << c->line() << "» to «" <<  t->name() << "»" << endl;
                                cout << ss.str();
                            });
                        // random traffic weight between [ 1, 10 ]
                        s->traffic_weight((1 + rand() % 10));
                        // add the branch
                        cj->add_branch() (s, HEAD);
                        // add the street to cluster
                        streets.push_back(s);
                        // check for cluster packing?
                        if(streets.size() >= CONST_CLUSTER_STREET_MAX_SIZE) {
                            this->_cluster_streets.push_back(streets);
                            streets.clear();
                        }
                    }
                    else
                        // not valid branch?
                        cj->add_branch()(street_ptr(nullptr), NILL);
                }
            }
            // add incoming streets to joint
            {
                // flags the condition that we can use a street
                bool flags[2] = { !is_first_col, !is_first_row };
                branch br[2] = { BRANCH_RIGHT, BRANCH_DOWN };
                vector<size_t> ind[2] = { { i, j-1 }, { i - 1, j } };
                FOR(k,0,2,++) {
                    if(flags[k])
                        cj->add_branch()(this->_blocks[ind[k][0]][ind[k][1]]->branches()[br[k]], TAIL);
                    else
                        // not valid branch?
                        cj->add_branch()(street_ptr(nullptr), NILL);
                }
            }
            assert(cj->size() == 4);
        }
    }
    // put the last remaining street cluster to pack!
    if(streets.size()) {
        assert(streets.size() < CONST_CLUSTER_STREET_MAX_SIZE);
        this->_cluster_streets.push_back(streets);
    }
}

city::~city()
{
    this->flow_stop();
    FOR(i,0,this->_height,++) delete[] this->_blocks[i];
    delete[] this->_blocks;
}

void city::flow_start() {
    this->_flow_stat = START;
    if(this->_flow_threads.size() == this->_cluster_streets.size()) return;
    // handles the cluster flowness
    auto flow_cluster = [](
        const vector<street_ptr>& cs,
        const std::atomic<flow_stat>& fs,
        const std::atomic<float>& delay_milliseconds,
        const std::atomic<float>& dt) {
        while(true) {
            if(fs == STOP) break;
            if(fs == PAUSE) { this_thread::sleep_for(chrono::milliseconds(100)); continue; }
            if(fs == START) {
                for(size_t i = 0; i < cs.size() && fs == START; i++)
                    cs.at(i)->flow(dt);
                this_thread::sleep_for(chrono::milliseconds(delay_milliseconds));
            }
        }
    };
    // dispatch a cluster flow thread
    FOR(i,0,this->_cluster_streets.size(),++) {
        this->_flow_threads.push_back(std::thread(
            flow_cluster,
            std::ref(this->_cluster_streets[i]),
            std::ref(this->_flow_stat),
            std::ref(this->_delay_milliseconds),
            std::ref(this->_dt)
        ));
        this->_flow_threads.back().detach();
    }
    assert(this->_flow_threads.size() == this->_cluster_streets.size());
}

void city::flow_stop() {
    this->_flow_stat = STOP;
    if(!this->_flow_threads.size()) return;
    this_thread::sleep_for(chrono::microseconds(500));
    this->_flow_threads.clear();
}

size_t city::add_cars(size_t count) {
    flow_stat fs = this->_flow_stat;

    // start the flow to make rooms for the new cars
    this->flow_start();

    size_t added_car = 0;

    // try as much as city can contain in one round
    count = min(count, this->get_size_streets() * 2 * CONST_STREET_LINES_NO - this->_car_no);

    FOR(i,0,count, ++) {
        size_t retry = 0;
        while(retry++ < this->_cluster_streets.size()) {
            // random select a cluster
            vector<street_ptr>& c = this->_cluster_streets[get_rand(0, this->_cluster_streets.size())];
            // fail-safe
            if(!c.size()) continue;
            // random select a street
            street_ptr s = c[get_rand(0, c.size())];
            // create a new car with unique ID
            car_ptr cc(new car("CAR-" + std::to_string(this->_car_no++)));
            // random direction
            cc->direction(course(int(round(frand()))));
            // random between [ 3.6km/h , 90km/h ]
            cc->max_speed(1 + get_rand(0, 25));
            // try to bound the car
            if(s->bound_car(cc, inverse_course(cc->direction())) && ++added_car)
                break;
            else { this->_car_no--; }
        }
    }

    // resume the flow status
    switch(fs) {
        case STOP: this->flow_stop(); break;
        case PAUSE: this->flow_pause(); break;
        case START: break;
        default: throw runtime_error("invalid flow stat!");
    }

    return added_car;
}

string city::status() const {
    int level = 0;
    stringstream ss;
    json_open_str("$ City", level++, ss, false);
    json_build_str("Joint#", std::to_string(this->_height) + "h x " + std::to_string(this->_weight) + "w", level, ss);
    json_build_str("Street#", this->get_size_streets(), level, ss);
    json_build_str("Car#", this->get_size_cars(), level, ss);
    json_build_str("Street Cluster#", this->get_size_cluster_street(), level, ss);
    json_build_str("Street Cluster Size", CONST_CLUSTER_STREET_MAX_SIZE, level, ss);
    json_open_str("Traffic Factor Stat.", level++, ss);
        traffic_factor_report tfr = this->traffic_report();
        json_build_str("Min", tfr.min, level, ss);
        json_build_str("Max", tfr.max, level, ss);
        json_build_str("Sum", tfr.sum, level, ss);
        json_build_str("Mean", tfr.avg, level, ss);
        json_build_str("Std", tfr.std, level, ss);
    json_close_str(--level, ss);
    json_close_str(--level, ss);
    return ss.str();
}

traffic_factor_report city::traffic_report(bool use_cache) const {
    static bool uninit = true;
    static traffic_factor_report tfr;
    if(uninit || !use_cache) {
        vector<float> v;
        FOR(i,0,this->_cluster_streets.size(),++) {
            vector<street_ptr> vf = this->_cluster_streets[i];
            for(street_ptr& s : vf) {
                v.push_back(s->traffic_weight());
                if(tfr.min > v.back()) tfr.min = v.back();
                if(tfr.max < v.back()) tfr.max = v.back();
                tfr.sum += v.back();
            }
        }
        tfr.avg = tfr.sum / v.size();
        tfr.std = std::sqrt(std::inner_product(v.begin(), v.end(), v.begin(), 0.0) / v.size() - tfr.avg * tfr.avg);
        uninit = false;
    }
    return tfr;
}
