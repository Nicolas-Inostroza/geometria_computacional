#include <iostream>
#pragma once  

template <typename T>


class Punto{
private:
    // Coordenadas del punto
    T x;
    T y;
    T z;
public:
    //Constructores
    Punto();
    Punto(T x, T y);
    Punto(T x, T y, T z);
    //Contructor copia
    Punto(const Punto<T> &p);
    Punto<T>& operator=(const Punto<T> &p) = default;
    //Getters
    T get_x() const;
    T get_y() const;
    T get_z() const;
    //Funciones
    double distancia(const Punto<T> &p);

    bool operator==(const Punto<T> &p);
    //print
    friend std::ostream& operator<<(std::ostream &os, const Punto<T> &p) {
        os << "(" << p.x << "," << p.y << "," << p.z << ")";
        return os;
    }

    ~Punto();
};
