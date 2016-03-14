#include "inc/base_event.hpp"
#include <thread>

base_event::base_event()
{ }

base_event::~base_event()
{ }

base_event& base_event::operator()(event id, event_handler callback) {
    if(!this->_events.count(id))
        this->_events.insert({id, vector<event_handler>()});
    this->_events[id].push_back(callback);
    return *this;
}

void base_event::fire(event id, const vector<event_callback_arg> args, bool async) const {
    if(!this->_events.count(id)) return;
    thread tr([&]() {
        auto vec = this->_events.at(id);
        for(event_handler& ec : vec) ec(args);
    });
    if(async)
        tr.detach();
    else
        tr.join();
}
