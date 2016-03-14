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
    float _traffic_weight_factor;
    vector<car_ptr> _cars[2][CONST_STREET_LINES_NO];
public:
    ~street();
    street(size_t, string = "?");

    void flow(float = 1, bool* = nullptr, bool* = nullptr);
    string status() const;
    joint*& joints(course);
    string to_string() const;
    bool inBoundCar(car_ptr, course);
    size_t size(course c) const;

    inline string name() const { return this->_name; }
    inline float& traffic_weight() { return this->_traffic_weight_factor; }
    inline float  traffic_weight() const { return this->_traffic_weight_factor; }
    inline size_t size() const { return this->size(HEAD) + this->size(TAIL); }
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
};

class joint
{
    vector<course> _end_courses;
    vector<street_ptr> _streets;
public:
    bool inBound(car_ptr, const street*);
    inline size_t size() const { return this->_streets.size(); }
    inline street_ptr operator[](size_t index) const { return this->_streets[index]; }
    inline void jointStreet(street_ptr s, course c) { s->joints(c) = this; this->_end_courses.push_back(c); this->_streets.push_back(s); }
};


#endif // STREET_H
