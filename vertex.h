#ifndef VERTEX_H_
#define VERTEX_H_

#include <iostream>
#include <cmath>
template<class T>
struct vertex {
    T x;
    T y;
};

template<class T>
std::istream& operator>>(std::istream& is, vertex<T>& p) {
    is >> p.x >> p.y;
    return is;
}

template<class T>
std::ostream& operator<<(std::ostream& os, vertex<T> p) {
    os << '(' << p.x << ' ' << p.y << ')';
    return os;
}

#endif //VERTEX_H
