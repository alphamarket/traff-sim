#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

#include "car.hpp"
#include "utilities.hpp"
#include <unordered_map>

class street
{
    const string _name;
    const float  _length;
    const size_t _capacity;
    float
        _head_last_pos  = INFINITY,
        _tail_last_pos  = INFINITY;
    unordered_map<car_id_t, car_ptr> _cars;
public:
    ~street();
    street(size_t, string = "?");

    void flow();
    string status() const;
    string to_string() const;
    bool inBoundCar(car_ptr);

    inline string name() const { return this->_name; }
    inline size_t size() const { return this->_cars.size(); }
    inline void outBoundCar(car_ptr c) { this->_cars.erase(c->getID()); }
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
};

#endif // STREET_H
