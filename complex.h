#ifndef COMPLEX_H
#define COMPLEX_H


class Complex {
public:
    Complex() = default;
    Complex(double x = 0, double y = 0): _x(x), _y(y){};
    Complex operator*(Complex other);
    Complex operator+(Complex other);
    double len_sqr();
//    Complex operator-(Complex other);
    double _x = 0, _y = 0;

private:

};

#endif // COMPLEX_H
