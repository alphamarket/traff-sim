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
    FOR(i,0,4,++) {
        car_ptr c(new car("C-"+to_string(i)));
        c->direction() = (i < 2 ? HEAD : TAIL);
        assert(s.inBoundCar(c));
        c->speed() = 4.8;
        cout<<"CAR "<<*c<<endl;
    }
    FOR(i,0,5,++) { s.flow(); cout<<s<<endl<<endl; }
    FOR(i,4,8,++) {
        car_ptr c(new car("C-"+to_string(i)));
        c->direction() = (i < 6 ? HEAD : TAIL);
        assert(s.inBoundCar(c));
        c->speed() = 20;
        cout<<"CAR "<<*c<<endl;
    }
    cout<<"----"<<endl;
    while(s.size()) {
        s.flow();
        cout<<s<<endl<<endl;
    }
    return EXIT_SUCCESS;
}
