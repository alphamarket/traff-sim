#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

#include "car.hpp"
#include "utilities.hpp"
#include "base_event.hpp"

#define CONST_STREET_LINES_NO 2

class joint;
class street;

typedef shared_ptr<joint> joint_ptr;
typedef shared_ptr<street> street_ptr;

class street : public base_event
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
    typedef enum event_flags {
        ON_EXIT
    } event_flags;
public:
    ~street();
    street(size_t, string = "?");

    string status() const;
    joint*& joints(course);
    string to_string() const;
    size_t size(course c) const;
    bool inBoundCar(car_ptr, course);
    void has_flow(bool* = nullptr, bool* = nullptr) const;
    void flow(float = 1, bool* = nullptr, bool* = nullptr);

    inline string name() const { return this->_name; }
    inline float& traffic_weight() { return this->_traffic_weight_factor; }
    inline float  traffic_weight() const { return this->_traffic_weight_factor; }
    inline size_t size() const { return this->size(HEAD) + this->size(TAIL); }
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
};

class joint : public base_event
{
    vector<course> _end_courses;
    vector<street_ptr> _branches;
public:
    bool dispatch(car_ptr, const street*);
    inline size_t size() const { return this->_branches.size(); }
    inline vector<street_ptr>& branches() { return this->_branches; }
    inline street_ptr operator[](size_t index) const { return this->_branches[index]; }
    inline void jointStreet(street_ptr s, course c) { s->joints(c) = this; this->_end_courses.push_back(c); this->_branches.push_back(s); }
    inline void dispatch_event(event name, event_callback callback) { FOR(i,0,this->_branches.size(),++) ((street_ptr)this->_branches[i])->add_event() (name, callback); }
    inline friend ostream& operator <<(ostream& os, const joint& j) { FOR(i,0,j.size(),++) os<<*j[i]<<endl; return os; }
};


#endif // STREET_H
