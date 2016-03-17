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
public:
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
    std::atomic<float> _time_step, _cluster_delay_milliseconds;
public:
    ~city();
    city(size_t height, size_t weight);
    string status() const;
    traffic_factor_report traffic_report(bool use_cache = true) const;
    void flow_stop();
    void flow_start();
    size_t add_cars(size_t count);
    inline size_t height() const { return this->_height; }
    inline size_t weight() const { return this->_weight; }
    inline void time_step(float ts) { this->_time_step = ts; }
    inline float time_step() const { return this->_time_step; }
    inline size_t get_size_cars() const { return this->_car_no; }
    inline flow_stat get_stat_flow() const { return this->_flow_stat; }
    inline size_t get_size_blocks() const { return this->_height * this->_weight; }
    inline float cluster_delay() const { return this->_cluster_delay_milliseconds; }
    inline void flow_pause() { if(this->_flow_stat == START) this->_flow_stat = PAUSE; }
    inline size_t get_size_cluster_street() const { return this->_cluster_streets.size(); }
    inline const vector<vector<street_ptr>>& cluster_streets() { return this->_cluster_streets; }
    inline void cluster_delay(float milliseconds) { this->_cluster_delay_milliseconds = milliseconds; }
    inline size_t get_size_streets() const { if(!this->_cluster_streets.size()) return 0; return (this->_cluster_streets.size() - 1) * CONST_CLUSTER_STREET_MAX_SIZE + this->_cluster_streets.back().size(); }
};

string to_string(city::flow_stat);

#endif // CITY_H
