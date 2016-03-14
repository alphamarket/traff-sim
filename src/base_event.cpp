#include "inc/base_event.hpp"
#include <thread>

base_event::base_event()
{ }

base_event::~base_event()
{ }

base_event& base_event::operator()(event name, event_callback callback) {
    if(!this->_events.count(name))
        this->_events.insert({name, vector<event_callback>()});
    this->_events[name].push_back(callback);
    return *this;
}

void base_event::fire(event name, const vector<event_callback_arg> args, bool detach) const {
    if(!this->_events.count(name)) return;
    thread tr([&]() {
        auto vec = this->_events.at(name);
        for(event_callback& ec : vec) ec(args);
    });
    if(detach)
        tr.detach();
    else
        tr.join();
}
