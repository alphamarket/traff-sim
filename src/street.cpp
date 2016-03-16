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
    // wait for thread lock get unlocked and thread guard for ever
    this->_mutex.lock();
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
            // thread guard
            this->_mutex.lock();
                FOR(line, 0, CONST_STREET_LINES_NO, ++)
                    s += this->_cars[c][line].size();
            this->_mutex.unlock();
            return s;
        default: invalid_course();
    }
}

void street::flow(float dt, bool* head_has_flow, bool* tail_has_flow) {
    bool has_flow[2 /* [HEAD,TAIL] */] = { false, false };
    // foreach direction
    FOR(dir, HEAD, TAIL + 1, ++) {
        // foreach line in directions
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            this->_mutex.lock();
            // fetch a line's car
            auto way = &this->_cars[dir][line];
            // foreach car in current direction/line
            FOR(i, 0, way->size(), ++) {
                car_ptr c = way->at(i);
                // the on hold flag
                bool _on_hold = false;
                // fail-check
                assert(c->direction() == dir);
                // check the next position the car will be if it's going with its max speed
                float _position = c->position() + c->max_speed() * dt;
                // if the max speed will create an accident with the front car
                if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                    // go as you can -- stop 10 centimeter apart from front car's back
                    _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong() /* 0.1m */;
                // if the car reached end of street(aka. reach to an end-joint)
                if(_position > this->_length) {
                    joint* _joint = nullptr;
                    // fetch the related end-joint
                    switch(c->direction()) {
                        case HEAD: _joint = this->_head_joint; break;
                        case TAIL: _joint = this->_tail_joint; break;
                        default: invalid_course();
                    }
                    // the next choosen street
                    street* t = nullptr;
                    // try to choose a route from bounded branches to the joint!
                    if(_joint && (t = _joint->dispatch_car(c, this))) {
                        // the car successfully dispatched and need to dismissed from current instance's inbound cars
                        way->erase(way->begin() + i--);
                        // fire the exit event
                        this->event_fire(street::AFTER_EXIT, {c.get(), this, t});
                    }
                    // if no joint or a failed dispatch?
                    else {
                        // fire the on traffic hold
                        this->event_fire(street::ON_TRAFFIC_HOLD, {c.get(), this});
                        // initiate the car hold procedure
                        goto __HOLD;
                    }
                    continue;
                __HOLD:
                    _on_hold = true;
                    // stop the car at nearest possible place to the joint
                    _position = this->_length;
                    if(i > 0 && _position >= way->at(i-1)->position() - way->at(i-1)->getLong())
                        _position = way->at(i-1)->position() - 0.1 /* 0.1m */ - way->at(i-1)->getLong();
                }
                has_flow[dir] = has_flow[dir] || (!_on_hold && c->position() != _position);
                // update the position of car
                c->position(_position);
            }
            this->_mutex.unlock();
        }
    }
    // return the flowness flag of directions
    if(head_has_flow) *head_has_flow = has_flow[HEAD];
    if(tail_has_flow) *tail_has_flow = has_flow[TAIL];
}

void street::is_road_block(bool* head_has_flow, bool* tail_has_flow) const {
    if(!head_has_flow && tail_has_flow) return;
    bool has_flow[2/* [HEAD, TAIL] */] = {false, false};
    // thread guard
    this->_mutex.lock();
        FOR(dir, HEAD, TAIL + 1, ++)
            FOR(line, 0, CONST_STREET_LINES_NO, ++)
                has_flow[dir] = has_flow[dir] ||
                    (this->_cars[dir][line].size() && this->_cars[dir][line].front()->position() < this->_length);
    this->_mutex.unlock();
    if(head_has_flow) *head_has_flow = has_flow[HEAD] ;
    if(tail_has_flow) *tail_has_flow = has_flow[TAIL] ;
}

bool street::bound_car(car_ptr c, course from) {
    // unknown line
    int line = -1;
    // current direction of car
    course dir = c->direction();
    // cars which enter from a direction should go to the opposite direction!
    if(from == dir) dir = inverse_course(dir);
    // for each line in current direction
    FOR(_line, 0, CONST_STREET_LINES_NO, ++) {
        // if the capacity not reached
        if( this->_cars[dir][_line].size() < this->_capacity &&
        // if there is place to enter the street
            (this->_cars[dir][_line].empty() || this->_cars[dir][_line].back()->position() > c->getLong() / 2)) {
            // pick the line
            line = _line;
            break;
        }
    }
    // if the line still unkown? no room to enter!!
    if(line == -1) return false;
    // adapt the current street's situation
    c->line(line);
    c->position(0);
    c->direction(dir);
    // make an impact on the car's tour history
    c->add2Tour(this->_name);
    // validate direction
    if(c->direction() != HEAD && c->direction() != TAIL) invalid_course();
    // accept the car
    this->_mutex.lock();
        this->_cars[c->direction()][c->line()].push_back(c);
    this->_mutex.unlock();
    // flag the successfull adaption
    return true;
}

joint*& street::joints(course c) {
    switch(c) {
        case HEAD: return this->_head_joint;
        case TAIL: return this->_tail_joint;
        default: invalid_course();
    }
}
