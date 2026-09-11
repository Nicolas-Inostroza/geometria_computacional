#include "punto.hpp"
#pragma once  
template <typename T>

class Vector{
    private:
        // Punto de origen del vector
        Punto<T> origen;
    public:
        // Constructores
        Vector();
        Vector(T x, T y);
        Vector(T x, T y, T z);
        Vector(const Punto<T> &p);

        // Copia
        Vector(const Vector<T> &v);

        //Getter
        Punto<T> get_origen();

        // Funciones
        double magnitud();
        double producto_punto(const Vector<T> &v);
        Vector<T> producto_cruz(const Vector<T> &v);

        // Overloads
        Vector<T> operator+(const Vector<T> &v);
        bool operator==(const Vector<T> &v);
        Vector<T> operator*(T escalar);

        // Print
        friend std::ostream &operator<<(std::ostream &os, const Vector<T> &v){
            os << v.origen;
            return os;
        };

        ~Vector();
};