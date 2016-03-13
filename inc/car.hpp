#ifndef CAR_H
#define CAR_H

#include "stdafx.hpp"
#include "utilities.hpp"

typedef string car_id_t;

#define CONST_AVG_CAR_LONG 4.826

class car
{
    int _line;
    float _pos;
    course _dir;
    float _speed;
    car_id_t _car_id;
public:
    ~car();
    car(car_id_t);
    string to_string() const;
    inline car_id_t getID() const { return this->_car_id; }
    inline course& direction() { return this->_dir; }
    inline course direction() const { return this->_dir; }
    inline float& position() { return this->_pos; }
    inline float  position() const { return this->_pos; }
    inline float  getLong() const { return CONST_AVG_CAR_LONG; }
    inline float  speed() const { return this->_speed; }
    inline float& speed() { return this->_speed; }
    inline int& line() { return this->_line; }
    inline int  line() const { return this->_line; }
    inline friend ostream& operator <<(ostream& os, const car& c) { os << c.to_string(); return os; }
};

typedef shared_ptr<car> car_ptr;

#endif // CAR_H
