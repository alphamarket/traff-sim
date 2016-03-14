#ifndef CAR_H
#define CAR_H

#include "stdafx.hpp"
#include "utilities.hpp"

typedef string car_id_t;

#define CONST_AVG_CAR_LONG (float(4.826))

class car
{
    int _line;
    float _pos;
    course _dir;
    float _max_speed;
    car_id_t _car_id;
    vector<string> _tour_history;
public:
    ~car();
    car(car&&);
    car(car_id_t);
    car(const car&);
    car& operator = (const car&);
    string to_string() const;
    inline car_id_t getID() const { return this->_car_id; }
    inline void   direction(course d) { this->_dir = d; }
    inline course direction() const { return this->_dir; }
    inline void position(float p) { this->_pos = p; }
    inline float  position() const { return this->_pos; }
    inline float  getLong() const { return CONST_AVG_CAR_LONG; }
    inline float  max_speed() const { return this->_max_speed; }
    inline void max_speed(float s) { this->_max_speed = s; }
    inline void line(int l) { this->_line = l; }
    inline int  line() const { return this->_line; }
    inline vector<string> getTour() const { return this->_tour_history; }
    inline void add2Tour(string flag) { this->_tour_history.push_back(flag + ", LIN: " + std::to_string(this->_line) + ", DIR: " + ::to_string(this->_dir)); }
    inline friend ostream& operator <<(ostream& os, const car& c) { os << c.to_string(); return os; }
};

typedef shared_ptr<car> car_ptr;

#endif // CAR_H
