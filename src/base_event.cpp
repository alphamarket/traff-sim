#include "inc/base_event.hpp"
#include <thread>

base_event& base_event::operator()(event id, event_handler callback) {
    if(!this->_events.count(id))
        this->_events.insert({id, vector<event_handler>()});
    this->_events[id].push_back(callback);
    return *this;
}

void base_event::event_fire(event id, const vector<event_callback_arg> args, bool async) const {
    if(!this->_events.count(id)) return;
#ifndef QTCTREATOR
    thread tr([&]() {
#else
        // a warning free work-around to un-used arg.
        static_cast<void>(async);
#endif
        this->_mutex.lock();
            auto vec = this->_events.at(id);
            for(event_handler& ec : vec) ec(args);
        this->_mutex.unlock();
#ifndef QTCTREATOR
    });
    if(async)
        tr.detach();
    else
        tr.join();
#endif
}
