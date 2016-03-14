#include "inc/street.hpp"

#include <string.h>
#include <algorithm>

street::street(size_t car_capacity, string name)
    : _name(name), _length(car_capacity * CONST_AVG_CAR_LONG), _capacity(car_capacity),
      _head_joint(nullptr), _tail_joint(nullptr),
      _traffic_weight_factor(1)
{
}

street::~street()
{
}

string street::to_string() const {
    stringstream ss;
    ss << this->_name << " St. { cap: " << this->_capacity << ", len: " << this->_length << "m, twf: "<<this->_traffic_weight_factor<<" }";
    return ss.str();
}

string street::status() const {
    stringstream ss;
    ss  << "[Status]" << endl << this->to_string() << endl
        << "car#" << this->size() << " last_pos: { "; //<< "m, T:" << this->_tail_last_pos << "m }";

    FOR(dir, HEAD, TAIL + 1, ++) {
        ss << ::to_string(course(dir)) << "[";
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            ss << this->_cars[dir][line].size();
            if(line < CONST_STREET_LINES_NO - 1) ss << "," << (this->_cars[dir][line].size() ? " " : "");
        }
        ss << "]" << ": {";
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            if(this->_cars[dir][line].size()) {
                ss << this->_cars[dir][line].back()->position();
            }
            if(line < CONST_STREET_LINES_NO - 1) ss << "," << (this->_cars[dir][line].size() ? " " : "");
        }
        ss << "}, ";
    }
    ss << "}";

    return ss.str();
}

size_t street::size(course c) const {
    size_t s = 0;
    switch (c) {
        case HEAD:
        case TAIL:
            FOR(line, 0, CONST_STREET_LINES_NO, ++)
                s += this->_cars[c][line].size();
            return s;
        default: invalid_course();
    }
}

void street::flow(float dt, bool* head_has_flow, bool* tail_has_flow) {
    int  overflow[2/* [HEAD, TAIL] */] = {0, 0};
    bool has_flow[2/* [HEAD, TAIL] */] = {true, true};
    FOR(dir, HEAD, TAIL + 1, ++) {
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            auto way = &this->_cars[dir][line];
            FOR(i, 0, way->size(), ++) {
                car_ptr c = way->at(i);
                assert(c->direction() == dir);
                float _position = c->position() + c->speed() * dt;
                if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                    _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong() /* 0.1m */;
                if(_position > this->_length) {
                    joint* _joint = nullptr;
                    switch(c->direction()) {
                        case HEAD: _joint = this->_head_joint; break;
                        case TAIL: _joint = this->_tail_joint; break;
                        default: invalid_course();
                    }
                    if(!_joint) { has_flow[dir] = false; goto __HOLD; }
                    if(_joint->inBound(c, this)) {
                        way->erase(way->begin() + i--);
                        // pass the car to the bound joint
                        cout<<"Car#: «" << c->getID() <<"» Dir: «" << ::to_string(c->direction()) << "» Line: «"<<c->line()<<"» Speed: «"<<c->speed()<<"» Exiting the: " << this->to_string() << endl;
                    } else { goto __HOLD; }
                    continue;
                __HOLD:
                    _position = this->_length;
                    if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                        _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong();
                    c->position(_position);
                    overflow[dir]++;
                } else c->position(_position);
            }
        }
    }
    if(head_has_flow) *head_has_flow = this->size(HEAD) && has_flow[HEAD] && overflow[HEAD] == 0;
    if(tail_has_flow) *tail_has_flow = this->size(TAIL) && has_flow[TAIL] && overflow[TAIL] == 0;
}

bool street::inBoundCar(car_ptr c, course from) {
    int line = -1;
    course dir = c->direction();
    if(from == dir) dir = inverse_course(dir);
    // cars which goes to head should enter from tail!
    FOR(_line, 0, CONST_STREET_LINES_NO, ++) {
        if( this->_cars[dir][_line].size() < this->_capacity &&
            (this->_cars[dir][_line].empty() || this->_cars[dir][_line].back()->position() > c->getLong() / 2)) {
            line = _line;
            break;
        }
    }
    if(line == -1) return false;
    c->line(line);
    c->position(0);
    c->direction(dir);
    c->add2Tour(this->_name);
    this->_cars[c->direction()][c->line()].push_back(c);
    return true;
}

joint*& street::joints(course c) {
    switch(c) {
        case HEAD: return this->_head_joint;
        case TAIL: return this->_tail_joint;
        default: invalid_course();
    }
}
bool joint::inBound(car_ptr c, const street* src) {
    size_t index = 0;
    vector<pair<size_t, float>> vs;
    float sum1 = 0, sum2 = 0, p = frand();
    for(street_ptr& s : this->_streets) {
        if(s.get() == src) vs.push_back(make_pair<size_t, float>(index++, 0));
        else vs.push_back(make_pair<size_t, float>(index++, exp(s->traffic_weight())));
        sum1 += vs.back().second;
    }
    sort(vs.begin(), vs.end(), [](pair<size_t, float> p1, pair<size_t, float> p2){ return p1.second < p2.second; });
    for(pair<size_t, float>& f : vs) {
        sum2 += (f.second / sum1);
        if(sum2 > p) return ((street_ptr)this->_streets[f.first])->inBoundCar(c, this->_end_courses[f.first]);
    }
    return false;
}
