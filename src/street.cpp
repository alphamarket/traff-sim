#include "inc/street.hpp"
#include <string.h>


street::street(size_t car_capacity, string name)
    : _name(name), _length(car_capacity * CONST_AVG_CAR_LONG), _capacity(car_capacity),
      _head_joint(nullptr), _tail_joint(nullptr),
      _traffic_weight(1)
{
    FOR(i, 0, CONST_STREET_LINES_NO, ++) this->_head_last_pos[i] = this->_tail_last_pos[i] = INFINITY;
}

street::~street()
{
}

string street::to_string() const {
    stringstream ss;
    ss << this->_name << " St. { cap#: " << this->_capacity << ", len: " << this->_length << "m }";
    return ss.str();
}

string street::status() const {
    stringstream ss;
    ss  << "[Status]" << endl << this->to_string() << endl
        << "car#" << this->_cars.size() << " last_pos: { " //<< "m, T:" << this->_tail_last_pos << "m }";
        << "H: {";
    FOR(i,0,CONST_STREET_LINES_NO,++) {
        ss << this->_head_last_pos[i];
        if(i < CONST_STREET_LINES_NO - 1) ss << ", ";
    }
    ss << "}, T: {";
    FOR(i,0,CONST_STREET_LINES_NO,++) {
        ss << this->_tail_last_pos[i];
        if(i < CONST_STREET_LINES_NO - 1) ss << ", ";
    }
    ss << "} }";
    return ss.str();
}

void street::flow() {
    float
        head_min_pos[CONST_STREET_LINES_NO],
        tail_min_pos[CONST_STREET_LINES_NO];
    std::fill(head_min_pos, head_min_pos + sizeof(head_min_pos), INFINITY);
    std::fill(tail_min_pos, tail_min_pos + sizeof(tail_min_pos), INFINITY);
    for(auto it = this->_cars.begin(); it != this->_cars.end();) {
        pair<car_id_t, car_ptr> cp = *it;
        car_ptr c = cp.second;
        c->position() += c->speed(); // moves it!!
        if(c->position() + c->getLong() / 2 > this->_length) {
            it = this->_cars.erase(it);
            switch(c->direction()) {
                // cars which goes to HEAD will add distance from tail and vice versa.
                case HEAD: if(this->_head_joint) this->_head_joint->inBound(c); break;
                case TAIL: if(this->_tail_joint) this->_tail_joint->inBound(c); break;
                default: invalid_course();
            }
            // pass the car to the bound joint
            cout<<"Car#: «" << c->getID() <<"» Dir: «" << ::to_string(c->direction()) << "» Exiting the: " << this->to_string() << endl;
        }
        else {
            switch(c->direction()) {
                // cars which goes to HEAD will add distance from tail and vice versa.
                case HEAD: tail_min_pos[c->line()] = min(c->position(), tail_min_pos[c->line()]); break;
                case TAIL: head_min_pos[c->line()] = min(c->position(), head_min_pos[c->line()]); break;
                default: invalid_course();
            }
            it++;
        }
    }
    if(!this->size()) {
        std::fill(head_min_pos, head_min_pos + sizeof(head_min_pos), INFINITY);
        std::fill(tail_min_pos, tail_min_pos + sizeof(tail_min_pos), INFINITY);
    }
    memcpy(this->_head_last_pos, head_min_pos, sizeof(head_min_pos));
    memcpy(this->_tail_last_pos, tail_min_pos, sizeof(tail_min_pos));
}

bool street::inBoundCar(car_ptr c) {
    if(this->_cars.count(c->getID())) throw runtime_error("The car already in bound!");
    if(this->_cars.size() >= this->_capacity) return false;
    c->line() = -1;
    switch(c->direction()) {
        case HEAD:
            // cars which goes to head should enter from tail!
            FOR(line, 0, CONST_STREET_LINES_NO, ++) {
                if(this->_tail_last_pos[line] > c->getLong() / 2) {
                    this->_tail_last_pos[line] = 0;
                    c->line() = line;
                    break;
                }
            }
            break;
        case TAIL:
            // cars which goes to tail should enter from head!
            FOR(line, 0, CONST_STREET_LINES_NO, ++) {
                if(this->_head_last_pos[line] > c->getLong() / 2) {
                    this->_head_last_pos[line] = 0;
                    c->line() = line;
                    break;
                }
            }
            break;
        default: invalid_course();
    }
    if(c->line() == -1) return false;
    c->position() = 0;
    this->_cars[c->getID()] = c;
    return true;
}

joint*& street::joints(course c) {
    switch(c) {
        case HEAD: return this->_head_joint;
        case TAIL: return this->_tail_joint;
        default: invalid_course();
    }
}

void joint::inBound(car_ptr c) {
    vector<float> vs;
    float sum1 = 0, sum2 = 0, p = frand();
    for(street_ptr& s : this->_streets) {
        vs.push_back(exp(s->traffic_weight()));
        sum1 += vs.back();
    }
    size_t index = 0;
    for(float& f : vs) {
        if(sum2 + (f / sum1) < p) {
            ((street_ptr)this->_streets[index])->inBoundCar(c);
            break;
        }
        index++;
        sum2 += (f / sum1);
    }
}
