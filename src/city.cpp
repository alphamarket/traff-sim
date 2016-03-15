#include "inc/city.hpp"

city::city(size_t height, size_t weight)
    : _height(height), _weight(weight)
{
    this->_blocks = new joint_ptr*[this->_height];
    // init joints
    FOR(i,0,this->_height,++) this->_blocks[i] = new joint_ptr[this->_weight];
    FOR(i,0,this->_height,++)
        FOR(j,0,this->_weight,++)
            this->_blocks[i][j] = joint_ptr(new joint("J["+std::to_string(i)+","+std::to_string(j)+"]"));
    // adding streets to joints
    // topology:
    //    +--+--+
    //    |  |  |
    //    +--+--+
    //    |  |  |
    //    +--+--+
    // joint's street indexing:
    //    RIGHT: 0
    //    DOWN:  1
    //    LEFT:  2
    //    UP:    3
    FOR(i,0,this->_height,++) {
        FOR(j,0,this->_weight,++) {
            // @ every instance at max create 2 streets [ RIGHT / DOWN ]
            //  > the last row only creates [ RIGHT ] street
            //  > the last column only creates [ DOWN ] street
            //  > the last(bottom right) joint don't creates anything!
            //  > streets are joint by their HEADs while creating
            // @ every joint at max uses the previous [ LEFT / UP ] joint's streets
            //  > the first row only uses the [ LEFT ] joint's [ RIGHT ] street
            //  > the first column only uses the [ UP ] joint's [ DOWN ] street
            //  > the first(top left) joint don't uses anything!
            //  > streets are joint by their TAILs while using
            // for not available branches `nullptr` will be assigned instead
            bool
                is_first_row   = (i == 0),
                is_last_row    = ((i + 1) == this->_height),
                is_first_col   = (j == 0),
                is_last_col    = ((j + 1) == this->_weight);
            // fetch the joint
            joint_ptr& cj = this->_blocks[i][j];
            // add outgoing street to joint
            {
                // flags the condition that we can create a street
                bool flags[2] = { !is_last_col, !is_last_row };
                string postfix[2] = { "-R", "-D" };
                FOR(k,0,2,++) {
                    if(flags[k]) {
                        // random capacity between [ 1km, 8km ]
                        size_t cap = ((1000 / CONST_AVG_CAR_LONG) + rand() % int(7000 / CONST_AVG_CAR_LONG));
                        street* s  = new street(cap, "S-"+cj->name() + postfix[k]);
                        // random traffic weight between [ 1, 10 ]
                        s->traffic_weight((1 + rand() % 10));
                        // add the branch
                        cj->add_branch() (street_ptr(s), HEAD);
                    }
                    else
                        // not valid branch?
                        cj->add_branch()(street_ptr(nullptr), NILL);
                }
            }
            // add incoming streets to joint
            {
                // flags the condition that we can use a street
                bool flags[2] = { !is_first_col, !is_first_row };
                branch br[2] = { BRANCH_RIGHT, BRANCH_DOWN };
                vector<size_t> ind[2] = { { i, j-1 }, { i - 1, j } };
                FOR(k,0,2,++) {
                    if(flags[k])
                        cj->add_branch()(this->_blocks[ind[k][0]][ind[k][1]]->branches()[br[k]], TAIL);
                    else
                        // not valid branch?
                        cj->add_branch()(street_ptr(nullptr), NILL);
                }
            }
            assert(cj->size() == 4);
        }
    }
}

city::~city()
{
    FOR(i,0,this->_height,++) delete[] this->_blocks[i];
    delete[] this->_blocks;
}

