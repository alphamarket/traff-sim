#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

#include "car.hpp"
#include "utilities.hpp"
#include <unordered_map>

#define CONST_STREET_LINES_NO 2

class joint;

class street
{
    const string _name;
    const float  _length;
    const size_t _capacity;
    joint
        *_head_joint,
        *_tail_joint;
    float
        _traffic_weight,
        _head_last_pos[CONST_STREET_LINES_NO],
        _tail_last_pos[CONST_STREET_LINES_NO];
    unordered_map<car_id_t, car_ptr> _cars;
public:
    ~street();
    street(size_t, string = "?");

    void flow();
    string status() const;
    joint*& joints(course);
    string to_string() const;
    bool inBoundCar(car_ptr);

    inline string name() const { return this->_name; }
    inline size_t size() const { return this->_cars.size(); }
    inline float& traffic_weight() { return this->_traffic_weight; }
    inline void outBoundCar(car_ptr c) { this->_cars.erase(c->getID()); }
    inline float  traffic_weight() const { return this->_traffic_weight; }
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
};

typedef shared_ptr<street> street_ptr;

class joint
{
    vector<street_ptr> _streets;
public:
    joint();
    ~joint();
    void inBound(car_ptr);
    inline void jointStreet(street_ptr s, course c) { s->joints(c) = this; this->_streets.push_back(s); }
};


#endif // STREET_H
