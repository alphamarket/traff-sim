#ifndef STREET_H
#define STREET_H

#include "stdafx.hpp"

#include "car.hpp"
#include "utilities.hpp"
#include "base_event.hpp"

#define CONST_STREET_LINES_NO 2

class joint;

class street : public base_event
{
    /**
     * @brief _name the name of street
     */
    const string _name;
    /**
     * @brief _length the length of street in meter
     */
    const float  _length;
    /**
     * @brief _capacity the capacity of street in car#
     */
    const size_t _capacity;
    /**
    * @brief _head_joint the head joint of current street that it's inbound to
    */
    joint
        *_head_joint,
    /**
    * @brief _tail_joint the tail joint of current street that it's inbound to
    */
        *_tail_joint;
    /**
     * @brief _traffic_weight_factor the traffic weight factor of current street, the higher it get the busier it gets
     */
    float _traffic_weight_factor;
    /**
     * @brief _cars the vector of cars in each line of each direction
     */
    vector<car_ptr> _cars[2 /* [ HEAD, TAIL ]*/][CONST_STREET_LINES_NO];
public:
    /**
     * @brief the event flags of current entity
     */
    typedef enum event_flags {
        AFTER_EXIT, ON_TRAFFIC_HOLD
    } event_flags;
public:
    ~street();
    /**
     * @brief street instantize a street
     * @param capacity the # of cars that this instance allowed to contain at max
     * @param name the name of street
     */
    street(size_t capacity, string name = "?");
    /**
     * @brief status get an status message of street
     */
    string status() const;
    /**
     * @brief joints get the bounded joint
     * @param c the direction of joint with respect to the street
     */
    joint*& joints(course c);
    /**
     * @brief to_string street to string
     */
    string to_string() const;
    /**
     * @brief size get the # of cars
     * @param c the direction which cars are going
     * @return the # of cars going at defined direction
     */
    size_t size(course c) const;
    /**
     * @brief bound_car bounds a car to current instance
     * @param c the car to be bound
     * @param d the direction which cars is going
     * @return true if bound was successful; otherwise false
     */
    bool bound_car(car_ptr c, course d);
    /**
     * @brief name get the name of street
     */
    inline string name() const { return this->_name; }
    /**
     * @brief length get length of the street in meter
     */
    inline float length() const { return this->_length; }
    /**
     * @brief capacity get car capacity of the street in car#
     */
    inline size_t capacity() const { return this->_capacity; }
    /**
     * @brief size get the total # of cars inbound to current street
     */
    inline size_t size() const { return this->size(HEAD) + this->size(TAIL); }
    /**
     * @brief traffic_weight set the street's traffic weight factor
     */
    inline void traffic_weight(float tw) { this->_traffic_weight_factor = tw; }
    /**
     * @brief traffic_weight get the street's traffic weight factor
     */
    inline float  traffic_weight() const { return this->_traffic_weight_factor; }
    /**
     * @brief has_flow check if the street has flow
     * @param head_has_flow [OUT] for head direction flowness
     * @param tail_has_flow [OUT] for tail direction flowness
     */
    void is_road_block(bool* head_has_flow = nullptr, bool* tail_has_flow = nullptr) const;
    /**
     * @brief flow make a flow step in the street
     * @param dt the time step in second
     * @param head_has_flow [OUT] for head direction flowness
     * @param tail_has_flow [OUT] for tail direction flowness
     * @event ON_TRAFFIC_HOLD(car* the_car, street* current_street)
     * @event AFTER_EXIT(car* the_car, street* old_street, street* target_street)
     */
    void flow(float dt = 1, bool* head_has_flow = nullptr, bool* tail_has_flow = nullptr);
    /**
     * @brief operator << the ostreat operator
     */
    inline friend ostream& operator <<(ostream& os, const street& s) { os << s.status(); return os; }
    /**
     * @brief operator () get vector of cars
     * @param c the target course
     * @param line the target line in the course
     */
    inline vector<car_ptr> operator ()(course c, size_t line) const
    { switch(c) { case HEAD: case TAIL: if(line >= CONST_STREET_LINES_NO) throw runtime_error("line# overflow!"); return this->_cars[c][line]; default: invalid_course(); } }
};

typedef shared_ptr<street> street_ptr;

#endif // STREET_H
