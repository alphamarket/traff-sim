#include "inc/joint.hpp"
#include <algorithm>

bool joint::dispatch(car_ptr c, const street* src) {
    size_t index = 0;
    vector<pair<size_t, float>> vs;
    float sum1 = 0, sum2 = 0, p = frand();
    for(street_ptr& s : this->_branches) {
        if(s.get() == src) vs.push_back(make_pair<size_t, float>(index++, 0));
        else vs.push_back(make_pair<size_t, float>(index++, exp(s->traffic_weight())));
        sum1 += vs.back().second;
    }
    sort(vs.begin(), vs.end(), [](pair<size_t, float> p1, pair<size_t, float> p2){ return p1.second < p2.second; });
    for(pair<size_t, float>& f : vs) {
        sum2 += (f.second / sum1);
        if(sum2 > p) return ((street_ptr)this->_branches[f.first])->inBoundCar(c, this->_end_courses[f.first]);
    }
    return false;
}
