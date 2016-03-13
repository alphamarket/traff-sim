#include "inc/stdafx.hpp"

#include "inc/street.hpp"

int main(int, char**) {
    // city contains some joints connecting the streets
    // streets contain zero or more cars
    // streets are two-way in each direction
    // streets DRIVES the cars that are bound to them
    // a traffic report sent from streets/joints to TCU
    // TCU monitors/predicts/changes the lights/reports status
    street s(10, "s0");
    FOR(i,0,2,++) {
        car_ptr c(new car("C-"+to_string(i)));
        c->direction() = (i % 2 ? HEAD : TAIL);
        s.inBoundCar(c);
        c->speed() = 10 * (i + 1);
        cout<<"CAR "<<*c<<endl;
    }
    cout<<"----"<<endl;
    while(s.size()) {
        s.flow();
        cout<<s<<endl<<endl;
    }
    return EXIT_SUCCESS;
}
