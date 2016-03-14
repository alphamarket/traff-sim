#ifndef CAR_H
#define CAR_H

#include "stdafx.hpp"
#include "utilities.hpp"

typedef string car_id_t;

#define CONST_AVG_CAR_LONG (float(4.826)) // in meter

class car
{
    /**
     * @brief _line the line of street that car's driving
     */
    int _line;
    /**
     * @brief _pos the position of the head of car with respect to the street that it's in
     */
    float _pos;
    /**
     * @brief _dir the direction which car goes
     */
    course _dir;
    /**
     * @brief _max_speed the max speed to car
     */
    float _max_speed;
    /**
     * @brief _car_id the id of car
     */
    car_id_t _car_id;
    /**
     * @brief _tour_history the cars tour history container
     */
    vector<string> _tour_history;
public:
    ~car();
    /**
     * @brief car move ctor
     */
    car(car&&);
    /**
     * @brief car ctor
     * @param car_id the car's ID
     */
    car(car_id_t car_id);
    /**
     * @brief car copy ctor
     */
    car(const car&);
    /**
     * @brief to_string convert car to string
     */
    string to_string() const;
    /**
     * @brief operator = assginment opertator
     */
    car& operator = (const car&);
    /**
     * @brief line set the driving line
     */
    inline void line(int l) { this->_line = l; }
    /**
     * @brief line get th driving line
     */
    inline int  line() const { return this->_line; }
    /**
     * @brief position set the position of cars front with respect to container street
     */
    inline void position(float p) { this->_pos = p; }
    /**
     * @brief position get the position of cars front with respect to container street
     */
    inline float  position() const { return this->_pos; }
    /**
     * @brief direction set the driving direction
     */
    inline void   direction(course d) { this->_dir = d; }
    /**
     * @brief direction get the driving direction
     */
    inline course direction() const { return this->_dir; }
    /**
     * @brief direction get the ID# of car
     */
    inline car_id_t getID() const { return this->_car_id; }
    /**
     * @brief direction set the driving max speed
     */
    inline void max_speed(float s) { this->_max_speed = s; }
    /**
     * @brief direction get the length of car in meters
     */
    inline float  getLong() const { return CONST_AVG_CAR_LONG; }
    /**
     * @brief max_speed get the driving max speed
     */
    inline float  max_speed() const { return this->_max_speed; }
    /**
     * @brief getTour get the driving tour
     */
    inline vector<string> getTour() const { return this->_tour_history; }
    /**
     * @brief operator << the ostream operator
     */
    inline friend ostream& operator <<(ostream& os, const car& c) { os << c.to_string(); return os; }
    /**
     * @brief add2Tour add a driving tour history
     */
    inline void add2Tour(string history) { this->_tour_history.push_back("«" + history + "», LIN: «" + std::to_string(this->_line) + "», DIR: «" + ::to_string(this->_dir) + "»"); }
};

typedef shared_ptr<car> car_ptr;

#endif // CAR_H
