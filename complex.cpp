#include "complex.h"

Complex Complex::operator*(Complex other){
    Complex ans(0, 0);
    ans._x = this->_x * other._x - this->_y * other._y;
    ans._y = this->_x * other._y + other._x * this->_y;
    return  ans;
}

Complex Complex::operator+(Complex other){
    Complex ans(0, 0);
    ans._x = this->_x + other._x;
    ans._y = this->_y + other._y;
    return  ans;
}

double Complex::len_sqr(){
    return this->_x * this->_x + this->_y * this->_y;
}
