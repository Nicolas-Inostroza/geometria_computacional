#include "vector.hpp"

template <typename T>
Vector<T>::Vector(){
    this->origen=Punto<T>();
};

template <typename T>
Vector<T>::Vector(T x, T y){
    this->origen=Punto<T>(x,y);
};

template <typename T>
Vector<T>::Vector(T x, T y, T z){
    this->origen=Punto<T>(x,y,z);
};

template <typename T>
Vector<T>::Vector(const Punto<T> &p){
    this->origen=p;
};

template <typename T>
Vector<T>::Vector(const Vector<T> &v){
    this->origen=v.origen;
};

template <typename T>
Punto<T> Vector<T>::get_origen(){
    return this->origen;
};

template <typename T>
double Vector<T>::magnitud(){
    double mag = sqrt(pow(this->origen.get_x(),2)+pow(this->origen.get_y(),2)+pow(this->origen.get_z(),2));
    return mag;
};

template <typename T>
double Vector<T>::producto_punto(const Vector<T> &v){
    double prod = this->origen.get_x()*v.origen.get_x() + this->origen.get_y()*v.origen.get_y() + this->origen.get_z()*v.origen.get_z();
    return prod;
};

template <typename T>
Vector<T> Vector<T>::producto_cruz(const Vector<T> &v){
    double x_resultado = this->origen.get_y()*v.origen.get_z() - this->origen.get_z()*v.origen.get_y();
    double y_resultado = this->origen.get_z()*v.origen.get_x() - this->origen.get_x()*v.origen.get_z();
    double z_resultado = this->origen.get_x()*v.origen.get_y() - this->origen.get_y()*v.origen.get_x();
    return Vector<T>(x_resultado,y_resultado,z_resultado);
};

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T> &v){
    return Vector<T>(this->origen.get_x()+v.origen.get_x(),this->origen.get_y()+v.origen.get_y(),this->origen.get_z()+v.origen.get_z());
};

template <typename T>
bool Vector<T>::operator==(const Vector<T> &v){
    return (this->origen==v.origen);
};

template <typename T>
Vector<T> Vector<T>::operator*(T escalar){
    T x_resultado = this->origen.get_x() * escalar;
    T y_resultado = this->origen.get_y() * escalar;
    T z_resultado = this->origen.get_z() * escalar;
    return Vector<T>(x_resultado,y_resultado,z_resultado);
};

template <typename T>
Vector<T>::~Vector()=default;