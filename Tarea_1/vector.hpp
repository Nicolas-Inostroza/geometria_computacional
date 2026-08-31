#include "punto.hpp"
#pragma once  
template <typename T>

class Vector{
    private:
        Punto<T> origen;
    public:
        Vector();
        Vector(T x, T y);
        Vector(T x, T y, T z);
        Vector(const Punto<T> &p);
        Vector(const Vector<T> &v);
        
        Punto<T> get_origen();
        double magnitud();

        double producto_punto(const Vector<T> &v);
        Vector<T> producto_cruz(const Vector<T> &v);

        Vector<T> operator+(const Vector<T> &v);
        bool operator==(const Vector<T> &v);
        Vector<T> operator*(T escalar);

        friend std::ostream &operator<<(std::ostream &os, const Vector<T> &v){
            os << v.origen;
            return os;
        };

        ~Vector();
};