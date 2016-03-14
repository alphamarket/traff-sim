#include "inc/street.hpp"

#include <string.h>
#include <algorithm>

#include "inc/joint.hpp"

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
    json_open_str("Street", 0, ss);
    json_build_str("Name", this->_name, 1, ss);
    json_build_str("Length", this->_length, 1, ss);
    json_build_str("Traff. W. Factor", this->_traffic_weight_factor, 1, ss);
    json_close_str(0, ss);
    return ss.str();
}

string street::status() const {
    int level = 0;
    stringstream ss;
    json_open_str("$ Street", level++, ss);
    json_build_str("Name", this->_name, level, ss);
    json_build_str("Length", this->_length, level, ss);
    json_build_str("Traff. W. Factor", this->_traffic_weight_factor, level, ss);
    json_open_str("Status", level++, ss);
    json_build_str("Car#", this->size(), level, ss);
    json_open_str("Positions", level++, ss);

    FOR(dir, HEAD, TAIL + 1, ++) {
        string buf;
        {
            stringstream ss;
            ss << "[";
            FOR(line, 0, CONST_STREET_LINES_NO, ++) {
                ss << this->_cars[dir][line].size();
                if(line < CONST_STREET_LINES_NO - 1) ss << ",";
            }
            ss << "]" << ": { ";
            FOR(line, 0, CONST_STREET_LINES_NO, ++) {
                if(this->_cars[dir][line].size()) {
                    ss << this->_cars[dir][line].back()->position();
                }
                if(line < CONST_STREET_LINES_NO - 1) ss << " , ";// << (this->_cars[dir][line].size() ? " " : " ");
            }
            ss << " }";
            buf = ss.str();
        }
        json_build_str(::to_string(course(dir)), buf, level, ss, false);
    }
    json_close_str(--level, ss);
    json_close_str(--level, ss);
    json_close_str(--level, ss);
    assert(level == 0);
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
    FOR(dir, HEAD, TAIL + 1, ++) {
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            auto way = &this->_cars[dir][line];
            FOR(i, 0, way->size(), ++) {
                car_ptr c = way->at(i);
                assert(c->direction() == dir);
                float _position = c->position() + c->max_speed() * dt;
                if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                    _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong() /* 0.1m */;
                if(_position > this->_length) {
                    joint* _joint = nullptr;
                    switch(c->direction()) {
                        case HEAD: _joint = this->_head_joint; break;
                        case TAIL: _joint = this->_tail_joint; break;
                        default: invalid_course();
                    }
                    // pass the car to the bound joint to dispatch!
                    if(_joint && _joint->dispatch(c, this)) {
                        way->erase(way->begin() + i--);
                        this->fire(street::ON_EXIT, {c.get()});
                    } else { goto __HOLD; }
                    continue;
                __HOLD:
                    _position = this->_length;
                    if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                        _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong();
                    c->position(_position);
                } else c->position(_position);
            }
        }
    }
    this->has_flow(head_has_flow, tail_has_flow);
}

void street::has_flow(bool* head_has_flow, bool* tail_has_flow) const {
    bool has_flow[2/* [HEAD, TAIL] */] = {false, false};
    FOR(dir, HEAD, TAIL + 1, ++)
        FOR(line, 0, CONST_STREET_LINES_NO, ++)
            has_flow[dir] = has_flow[dir] ||
                            (this->_cars[dir][line].size() && this->_cars[dir][line].front()->position() < this->_length);
    if(head_has_flow) *head_has_flow = has_flow[HEAD] ;
    if(tail_has_flow) *tail_has_flow = has_flow[TAIL] ;
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
