#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

class Car;
class Joint;

class Street
{
    vector<shared_ptr<Car>> _cars;
    const string _name;
    shared_ptr<Joint> _joint[2];
public:
    ~Street();
    Street(const string = "?");
    inline string getName() const { return this->_name; }
    inline void boundCar(shared_ptr<Car> c) { this->_cars.push_back(c); }
    inline const shared_ptr<Joint>* getJoint() const { return this->_joint; }
    inline const vector<shared_ptr<Car>>& getCars() const { return this->_cars; }
    inline void boundJoint(shared_ptr<Joint> j1, shared_ptr<Joint> j2) { this->_joint[0] = j1; this->_joint[1] = j2; }
};

#endif // STREET_H
