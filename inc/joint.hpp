#ifndef JOINT_H
#define JOINT_H

#include "stdafx.hpp"
#include "street.hpp"
#include "utilities.hpp"
#include "base_event.hpp"

class joint : public base_event
{
    /**
     * @brief _mutex a mutex instance for thread safing stuff
     */
    mutable std::mutex _mutex;
    /**
     * @brief _name the name of the joint
     */
    const string _name;
    /**
     * @brief _end_courses the end course of the streets bound to current joint
     */
    vector<course> _end_courses;
    /**
     * @brief _branches all branches bound to current joint
     */
    vector<street_ptr> _branches;
public:
    /**
     * @brief joint move ctor
     */
    joint(joint&&);
    /**
     * @brief joint copy ctor
     */
    joint(const joint&);
    /**
     * @brief joint ctor a joint
     * @param name the name of joint
     */
    joint(const string name = "?") : _name(name) { }
    /**
     * @brief add_branch an add branch interface
     * @return current instance
     */
    inline joint& add_branch() { return *this; }
    /**
     * @brief name get name of the joint
     */
    inline string name() const { return this->_name; }
    /**
     * @brief dispatch dispatches a car from a street source
     * @param c the car to be dispatched
     * @param src the source street that car's coming from
     * @return true if dispatch was successful and the car placed in a street inbound to current joint; otherwise false
     */
    virtual street* dispatch_car(car_ptr c, const street* src);
    /**
     * @brief size get the number of inbound branches
     */
    inline size_t size() const { return this->_branches.size(); }
    /**
     * @brief branches get a vector of branches
     */
    inline vector<street_ptr>& branches() { return this->_branches; }
    /**
     * @brief operator [] access a branch by it' index#
     * @param index the index# of branch
     * @return the branch
     */
    inline street_ptr operator[](size_t index) const { return this->_branches[index]; }
    /**
     * @brief operator << ostream operator
     */
    inline friend ostream& operator <<(ostream& os, const joint& j) { FOR(i,0,j.size(),++) os<<*j[i]<<endl; return os; }
    /**
     * @brief operator () join a street as a branch to current joint
     * @param s the street
     * @param c the end-point of street which will going to attach to current joint
     * @return the current instance
     */
    inline joint& operator()(street_ptr s, course c) { this->_mutex.lock(); if(s) s->joints(c) = this; this->_end_courses.push_back(c); this->_branches.push_back(s); this->_mutex.unlock(); return *this; }
    /**
     * @brief dispatch_event dispatches an event to all branches of curren instance
     * @param id the event id
     * @param callback the event handler
     */
    inline void dispatch_event(event id, event_handler callback) { FOR(i,0,this->_branches.size(),++) ((street_ptr)this->_branches[i])->event_add() (id, callback); }
};

typedef shared_ptr<joint> joint_ptr;

/**
 * @brief The joint_kill class, never dispatches a car
 */
class joint_hold : public joint {
public:
    street* dispatch_car(car_ptr, const street*) override { return nullptr; }
};

/**
 * @brief The joint_kill class, kills every car that passed to be dispatched
 */
class joint_kill : public joint {
public:
    street* dispatch_car(car_ptr, const street *src) override { return const_cast<street*>(src); }
};


#endif // JOINT_H
