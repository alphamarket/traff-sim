#ifndef JOINT_H
#define JOINT_H

#include "stdafx.hpp"
#include "Street.hpp"

class Joint
{
    const size_t _size;
    shared_ptr<Street>* _s;
public:
    Joint(const size_t size) : _size(size) { this->_s = new shared_ptr<Street>[size]; }
    ~Joint() noexcept { delete[] this->_s; }
    inline Street& operator[](const size_t index)
    { if(index < this->_size) return *this->_s[index]; throw runtime_error("index out of range!"); }
    inline Street operator[](const size_t index) const
    { if(index < this->_size) return *this->_s[index]; throw runtime_error("index out of range!"); }
};

#endif // JOINT_H
