#include "punto.hpp"
#include <cassert>
#include <cmath>
#pragma once  

template <typename T>
Punto<T>::Punto(){
    this->x=0;
    this->y=0;
    this->z=0;
}

template <typename T>
Punto<T>::Punto(T x_valor, T y_valor) {
    this->x = x_valor;
    this->y = y_valor;
    this->z = 0;
}

template <typename T>
Punto<T>::Punto(T x_valor, T y_valor, T z_valor) {
    this->x = x_valor;
    this->y = y_valor;
    this->z = z_valor;
}

template <typename T>
Punto<T>::Punto(const Punto<T> &p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
}

template <typename T>
T Punto<T>::get_x() const{
    return this->x;
}
template <typename T>
T Punto<T>::get_y() const{
    return this->y;
}
template <typename T>
T Punto<T>::get_z() const{
    return this->z;
}

template <typename T>
double Punto<T>::distancia(const Punto<T> &p) {
    return sqrt(pow(this->x - p.x, 2) + pow(this->y - p.y, 2) + pow(this->z - p.z, 2));
}

template <typename T>
bool Punto<T>::operator==(const Punto<T>& p) {
    return (this->x == p.x && this->y == p.y && this->z == p.z);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Punto<T> &p) {
    os << "(" << p.x << "," << p.y << "," << p.z << ")";
    return os;
}

template <typename T>
Punto<T>::~Punto()=default;
