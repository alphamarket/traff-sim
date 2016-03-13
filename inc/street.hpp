#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

#include "car.hpp"
#include "utilities.hpp"

#define CONST_STREET_LINES_NO 2

class joint;
class street;

typedef shared_ptr<joint> joint_ptr;
typedef shared_ptr<street> street_ptr;

class street
{
    const string _name;
    const float  _length;
    const size_t _capacity;
    joint
        *_head_joint,
        *_tail_joint;
    float _traffic_weight;
    vector<car_ptr> _cars[2][CONST_STREET_LINES_NO];
public:
    ~street();
    street(size_t, string = "?");

    bool flow();
    string status() const;
    joint*& joints(course);
    string to_string() const;
    bool inBoundCar(car_ptr);
    size_t size(course c) const;

    inline string name() const { return this->_name; }
    inline float& traffic_weight() { return this->_traffic_weight; }
    inline float  traffic_weight() const { return this->_traffic_weight; }
    inline size_t size() const { return this->size(HEAD) + this->size(TAIL); }
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
};

class joint
{
    vector<street_ptr> _streets;
public:
    bool inBound(car_ptr, const street*);
    inline void jointStreet(street_ptr s, course c) { s->joints(c) = this; this->_streets.push_back(s); }
};


#endif // STREET_H
