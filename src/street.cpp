#include "inc/street.hpp"

street::street(size_t car_capacity, string name)
    : _name(name), _length(car_capacity * CONST_AVG_CAR_LONG), _capacity(car_capacity)
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
        << "car#" << this->_cars.size() << " last_pos: { H:" << this->_head_last_pos << "m, T:" << this->_tail_last_pos << "m }";
    return ss.str();
}

void street::flow() {
    float
        head_min_pos = INFINITY,
        tail_min_pos = INFINITY;
    for(auto it = this->_cars.begin(); it != this->_cars.end();) {
        pair<car_id_t, car_ptr> cp = *it;
        car_ptr c = cp.second;
        c->position() += c->speed(); // moves it!!
        if(c->position() + c->getLong() / 2 > this->_length) {
            it = this->_cars.erase(it);
            // pass the car to the bound joint
            cout<<"Car#: «" << c->getID() <<"» Dir: «" << ::to_string(c->direction()) << "» Exiting the: " << this->to_string() << endl;
        }
        else {
            switch(c->direction()) {
                case HEAD: head_min_pos = min(c->position(), head_min_pos); break;
                case TAIL: tail_min_pos = min(c->position(), tail_min_pos); break;
                default: throw runtime_error("Invalid direction!");
            }
            it++;
        }
    }
    if(!this->size()) { head_min_pos = tail_min_pos = INFINITY; }
    this->_head_last_pos = head_min_pos;
    this->_tail_last_pos = tail_min_pos;
}

bool street::inBoundCar(car_ptr c) {
    if(this->_cars.count(c->getID()))
        throw runtime_error("The car already in bound!");
    if(this->_cars.size() >= this->_capacity)
    { return false; }
    switch(c->direction()) {
        case HEAD:
            if(this->_head_last_pos < c->getLong() / 2) return false;
            break;
        case TAIL:
            if(this->_tail_last_pos < c->getLong() / 2) return false;
            break;
        default: throw runtime_error("Invalid direction!");
    }
    c->position() = 0;
    this->_cars[c->getID()] = c;
    return true;
}
