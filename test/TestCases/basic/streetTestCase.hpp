#ifndef STREETTESTCASE_HPP
#define STREETTESTCASE_HPP

#include "testCase.hpp"
#include "teststrap.hpp"
#include "inc/street.hpp"

using namespace CPP_TESTER;

namespace CPP_TESTER { namespace TESTS { namespace BASIC {

    class streetTestCase : public testCase {
    public:
        void run(size_t = 0, void** = NULL) {
            // create a street
            street s(10, "S0");
            bool head_flow, tail_flow;
            // test defaults
            IS_ZERO(s.size());
            IS_NULL(s.joints(HEAD));
            IS_NULL(s.joints(TAIL)); // no joint defined!
            SHOULD_BE(s.name(), "S0");
            SHOULD_BE(s.capacity(), 10);
            SHOULD_BE(s.traffic_weight(), 1);
            IS_ZERO(s.size(HEAD) + s.size(TAIL));
            SHOULD_BE(s.length(), s.capacity() * CONST_AVG_CAR_LONG);
            s.is_road_block(&head_flow, &tail_flow);
            IS_FALSE(head_flow || tail_flow); // there is no flow when there is no car!!
            // adding cars to the street
            FOR(i,0,10,++) {
                // create and init a car
                car_ptr c(new car("C-"+std::to_string(i)));
                c->max_speed(10 * (i >= 5 ? 2 : 1));
                c->direction(i < 5 ? HEAD : TAIL);
                // make the car bound
                auto r = s.bound_car(c, (i >= 5 ? HEAD : TAIL));
                // only two of each direction will be bound successfully
                if(i < 2 || (i >= 5 && i < 7)) IS_TRUE(r);
                else IS_FALSE(r);
            }
            // there will be 4 cars in 4 line and 2 course
            SHOULD_BE(s.size(), 4);
            // 2 of them will go to HEAD
            SHOULD_BE(s.size(HEAD), 2);
            // 2 of them will go to TAIL
            SHOULD_BE(s.size(TAIL), 2);
            // event call counter
            size_t
                event_call_after_exit = 0,
                event_call_on_traffic_hold = 0;
            // add events
            s.event_add()
                (street::AFTER_EXIT, [&event_call_after_exit](vector<const void*> args) {
                    event_call_after_exit++;
                    SHOULD_BE(args.size(), 3);
                    FOR(i,0,args.size(),++) NOT_NULL(args[i]);
                })
                (street::ON_TRAFFIC_HOLD, [&event_call_on_traffic_hold](vector<const void*> args) {
                    event_call_on_traffic_hold++;
                    SHOULD_BE(args.size(), 2);
                    FOR(i,0,args.size(),++) NOT_NULL(args[i]);
                });
            // test event's registered currectly
            IS_TRUE(s.event_has_defined(street::AFTER_EXIT));
            IS_TRUE(s.event_has_defined(street::ON_TRAFFIC_HOLD));
            // try to follow the street 5 times
            FOR(i,0,5,++) {
                // flow the street
                SHOULD_NOT_THROW(s.flow(1, &head_flow, &tail_flow));
                // by speed of 10 and street's cap# 10:
                //  exactly by 5th iter the cars should reach the end of street in their direction
                if(i < 2) IS_TRUE(head_flow && tail_flow);
                // by speed of 20 and street's cap# 10:
                //  exactly by 3th iter the cars should reach the end of street in their direction
                else if(i < 4) IS_TRUE(head_flow && !tail_flow);
                // for 5th or greater iteration all of lines should be blocked
                else IS_FALSE(head_flow || tail_flow);
            }
            // because there is no joind inbound, no car exists!
            SHOULD_BE(event_call_after_exit, 0);
            // all of them will be on traffic hold, this number should not be equal to the # of cars in lines
            //  because every time they will be tested to move and since they cannot exit, the re-put in HOLD status
            //  hence the event will be re-called!
            IS_TRUE(event_call_on_traffic_hold > 0);
            // create and init a car with its engine off
            car_ptr c(new car("ENGINE-OFF"));
            c->line(0);
            // zero speed
            c->max_speed(0);
            c->direction(HEAD);
            IS_TRUE(s.bound_car(c, TAIL));
            // considering the car that is already in the end-edge of this line, we should have 2 car in HEAD line
            SHOULD_BE(s(HEAD, 0).size(), 2);
            // try to flow the street for a long time
            FOR(i,0,1000,++) {
                SHOULD_NOT_THROW(s.flow(1, &head_flow, &tail_flow));
                car_ptr x = s(HEAD, 0).back();
                SHOULD_BE(x->getID(), "ENGINE-OFF");
            }
            // the car should hold still
            IS_ZERO(c->position());
            IS_ZERO(c->max_speed());
            IS_FALSE(head_flow || tail_flow);
            // add a tiny velocity
            c->max_speed(1e-3);
            // no change should occur in cars' list in this line
            SHOULD_BE(s(HEAD, 0).size(), 2);
            FOR(i,0,100,++) {
                SHOULD_NOT_THROW(s.flow(10, &head_flow, &tail_flow));
                car_ptr x = s(HEAD, 0).back();
                SHOULD_BE(x->getID(), "ENGINE-OFF");
            }
            // everything should match-up with setted <speed> && <dt>
            IS_TRUE(head_flow && !tail_flow);
            IS_TRUE(round(c->position()) == float(1));
            SHOULD_BE(c->max_speed(), float(1e-3));
        }
    };

} } }

#endif // VECTOR3TESTCASE_HPP

