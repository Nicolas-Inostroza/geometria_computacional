#include <iostream>
#pragma once  

template <typename T>


class Punto{
private:
    T x;
    T y;
    T z;
public:
    Punto();
    Punto(T x, T y);
    Punto(T x, T y, T z);
    Punto(const Punto<T> &p);

    T get_x() const;
    T get_y() const;
    T get_z() const;

    double distancia(const Punto<T> &p);

    bool operator==(const Punto<T> &p);

    friend std::ostream& operator<<(std::ostream &os, const Punto<T> &p) {
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }

    ~Punto();
};

