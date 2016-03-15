#include "inc/joint.hpp"
#include <algorithm>


joint::joint(const joint & j)
    : base_event(j), _name(j._name)
{
    this->_branches = j._branches;
    this->_end_courses = j._end_courses;
}

joint::joint(joint&& j)
    : base_event(j), _name(move(j._name))
{
    this->_branches = move(j._branches);
    this->_end_courses = move(j._end_courses);
}

street* joint::dispatch_car(car_ptr c, const street* src) {
    // using softmax for computing the probability and roulette wheel to select a route
    size_t index = 0;
    vector<pair<size_t, float>> vs;
    float sum1 = 0, sum2 = 0, p = frand();
    // compute the softmax numbers
    for(street_ptr& s : this->_branches) {
        // the prob. of going back to current route is ZERO
        if(!s || s.get() == src) vs.push_back(make_pair<size_t, float>(index++, 0));
        // e ^ { route's traffic weight }
        else vs.push_back(make_pair<size_t, float>(index++, exp(s->traffic_weight())));
        // the softmax's sum denominator
        sum1 += vs.back().second;
    }
    // sort the branches with their select prob. ascending
    sort(vs.begin(), vs.end(), [](pair<size_t, float> p1, pair<size_t, float> p2){ return p1.second < p2.second; });
    // apply roulette wheel
    for(pair<size_t, float>& f : vs) {
        sum2 += (f.second / sum1);
        street_ptr t = (street_ptr)this->_branches[f.first];
        // if the prob. was satisfied and the car inbound to street successfully?
        if(sum2 > p && t && t->bound_car(c, this->_end_courses[f.first]))
            // return the street as selected street
            return t.get();
    }
    // return null if nothing selected SUCCESSFULLY
    return nullptr;
}
