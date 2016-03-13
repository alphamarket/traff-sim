#include "inc/street.hpp"
#include <string.h>


street::street(size_t car_capacity, string name)
    : _name(name), _length(car_capacity * CONST_AVG_CAR_LONG), _capacity(car_capacity),
      _head_joint(nullptr), _tail_joint(nullptr),
      _traffic_weight(1)
{
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
        << "car#" << this->size() << " last_pos: { "; //<< "m, T:" << this->_tail_last_pos << "m }";

    FOR(dir, HEAD, TAIL + 1, ++) {
        ss << ::to_string(course(dir)) << ": {";
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            if(this->_cars[dir][line].size()) {
                ss << this->_cars[dir][line].back()->position();
                if(line < CONST_STREET_LINES_NO - 1) ss << ", ";
            }
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

bool street::flow() {
    int has_flow = 2;
    FOR(dir, HEAD, TAIL + 1, ++) {
        FOR(line, 0, CONST_STREET_LINES_NO, ++) {
            auto way = &this->_cars[dir][line];
            FOR(i, 0, way->size(),++) {
                car_ptr c = way->at(i);
                // move as you can!!
                if(i > 0 && c->position() + c->speed() >= way->at(i-1)->position())
                    c->position() = way->at(i-1)->position() - 0.1 /* 0.1m */;
                else c->position() += c->speed();
                if(c->position() + c->getLong() / 2 > this->_length) {
                    joint* _joint = nullptr;
                    switch(c->direction()) {
                        case HEAD: _joint = this->_head_joint; break;
                        case TAIL: _joint = this->_tail_joint; break;
                        default: invalid_course();
                    }
                    if(_joint && this->_head_joint->inBound(c, this)) {
                        way->erase(way->begin() + i--);
                        // pass the car to the bound joint
                        cout<<"Car#: «" << c->getID() <<"» Dir: «" << ::to_string(c->direction()) << "» Line: «"<<c->line()<<"» Exiting the: " << this->to_string() << endl;
                    } else { has_flow--; line = CONST_STREET_LINES_NO; break; }
                }
            }
        }
    }
    return has_flow;
}

bool street::inBoundCar(car_ptr c) {
    c->line() = -1;
    // cars which goes to head should enter from tail!
    FOR(line, 0, CONST_STREET_LINES_NO, ++) {
        if( this->_cars[c->direction()][line].size() < this->_capacity &&
            (this->_cars[c->direction()][line].empty() || this->_cars[c->direction()][line].back()->position() > c->getLong() / 2)) {
            c->line() = line;
            break;
        }
    }
    if(c->line() == -1) return false;
    c->position() = 0;
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
    vector<float> vs;
    float sum1 = 0, sum2 = 0, p = frand();
    for(street_ptr& s : this->_streets) {
        if(s.get() == src) vs.push_back(0);
        else vs.push_back(exp(s->traffic_weight()));
        sum1 += vs.back();
    }
    size_t index = 0;
    for(float& f : vs) {
        if(sum2 + (f / sum1) > p) {
            return ((street_ptr)this->_streets[index])->inBoundCar(c);
            break;
        }
        index++;
        sum2 += (f / sum1);
    }
    return false;
}
