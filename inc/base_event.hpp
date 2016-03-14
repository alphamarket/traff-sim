#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "stdafx.hpp"
#include <functional>
#include <unordered_map>

typedef int event;
typedef const void* event_callback_arg;
typedef std::function<void(vector<event_callback_arg>)> event_callback;

class base_event
{
    unordered_map<event, vector<event_callback>> _events;
protected:
    void fire(event, const vector<event_callback_arg>, bool = false) const;
public:
    base_event();
    ~base_event();
    base_event& operator()(event, event_callback);
    inline base_event& add_event() { return *this; }
    inline void remove_event(event name) { this->_events.erase(name); }
};


#endif // BASE_EVENT_H
