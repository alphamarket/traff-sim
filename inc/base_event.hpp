#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "stdafx.hpp"
#include <functional>
#include <unordered_map>

typedef int event;
typedef const void* event_callback_arg;
typedef std::function<void(vector<event_callback_arg>)> event_handler;

class base_event
{
    /**
     * @brief _events the event container box
     */
    unordered_map<event, vector<event_handler>> _events;
protected:
    /**
     * @brief fire fires every event registered with the event-id
     * @param id the id of event
     * @param args the agument which will passed to event handlers
     * @param async should the event calls processed async. from current thread?
     */
    void event_fire(event id, const vector<event_callback_arg> args, bool async = false) const;
public:
    base_event();
    ~base_event();
    /**
     * @brief operator () registers a new event handler
     * @param id the event id
     * @param callback the handler
     * @return current instance
     */
    base_event& operator()(event id, event_handler callback);
    /**
     * @brief add_event and interface to add event
     * @return current instance
     */
    inline base_event& event_add() { return *this; }
    /**
     * @brief remove_event removes all handlers assigned to an event
     * @param id the event id
     */
    inline void event_remove(event id) { this->_events.erase(id); }
    /**
     * @brief event_exists check if an event already registered
     * @param id the event's id
     */
    inline bool event_has_defined(event id) const { return this->_events.count(id); }
};

#endif // BASE_EVENT_H
