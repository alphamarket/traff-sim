#include "inc/car.hpp"

car::car(car_id_t car_id)
    : _line(-1), _pos(-1), _dir(NILL), _max_speed(0), _car_id(car_id)
{ }

car::~car()
{ }

car::car(car&& c)
    : _line(move(c._line)), _pos(move(c._pos)), _dir(move(c._dir)),
    _max_speed(move(c._max_speed)), _car_id(move(c._car_id)), _tour_history(move(c._tour_history))
{ }

car::car(const car& c)
    : _line(c._line), _pos(c._pos), _dir(c._dir),
    _max_speed(c._max_speed), _car_id(c._car_id), _tour_history(c._tour_history)
{ }

car& car::operator = (const car& c) {
    this->_line = c._line;
    this->_pos = c._pos;
    this->_dir = c._dir;
    this->_max_speed = c._max_speed;
    this->_car_id = c._car_id;
    this->_tour_history = c._tour_history;
    return *this;
}

string car::to_string() const {
    stringstream ss;
    ss << "ID#: «" << this->getID() << "» DIR: «" << ::to_string(this->direction()) << "» LIN: «"<<this->_line<<"» SPD: «" << this->max_speed() << "» POS: «" << this->position() << "»";
    return ss.str();
}
