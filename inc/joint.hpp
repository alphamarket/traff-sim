#ifndef JOINT_H
#define JOINT_H

#include "stdafx.hpp"
#include "street.hpp"
#include "utilities.hpp"
#include "base_event.hpp"

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

#endif // JOINT_H
