#ifndef CITY_H
#define CITY_H

#include "stdafx.hpp"

#include <mutex>
#include <thread>
#include <atomic>

#include "joint.hpp"
#include "street.hpp"

#define CONST_CLUSTER_STREET_MAX_SIZE 50

struct traffic_factor_report {
    double max = -INFINITY, min = INFINITY, avg = 0, std = 0, sum = 0;
};

class city
{
protected:
    typedef enum branch {
        BRANCH_RIGHT = 0,
        BRANCH_DOWN = 1,
        BRANCH_LEFT = 2,
        BRANCH_UP   = 3
    } branch;
    typedef enum flow_stat {
        STOP,
        START,
        PAUSE
    } flow_stat;
private:
    const size_t
        _height,
        _weight;
    size_t
        _car_no;
    joint_ptr** _blocks;
    vector<thread> _flow_threads;
    std::atomic<flow_stat> _flow_stat;
    vector<mutex>  _flow_threads_mutex;
    vector<vector<street_ptr>> _cluster_streets;
    std::atomic<float> _dt, _delay_milliseconds;
public:
    ~city();
    city(size_t height, size_t weight);
    string status() const;
    traffic_factor_report traffic_report(bool use_cache = true) const;
    void flow_stop();
    void flow_start();
    size_t add_cars(size_t count);
    inline void flow_pause() { this->_flow_stat = PAUSE; }
    inline size_t get_size_cars() const { return this->_car_no; }
    inline size_t get_size_blocks() const { return this->_height * this->_weight; }
    inline size_t get_size_cluster_street() const { return this->_cluster_streets.size(); }
    inline size_t get_size_streets() const { if(!this->_cluster_streets.size()) return 0; return (this->_cluster_streets.size() - 1) * CONST_CLUSTER_STREET_MAX_SIZE + this->_cluster_streets.back().size(); }
};

#endif // CITY_H
