#include "inc/car.hpp"

car::car(car_id_t car_id)
    : _pos(0), _dir(NILL), _speed(30), _car_id(car_id)
{

}

car::~car()
{

}

string car::to_string() const {
    stringstream ss;
    ss << "ID#: «" << this->getID() << "» DIR: «" << ::to_string(this->direction()) << "» SPD: «" << this->speed() << "» POS: «" << this->position() << "»";
    return ss.str();
}
