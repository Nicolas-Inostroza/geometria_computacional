#include "punto.hpp"
#include <ostream>
#include <vector>
#pragma once

template <typename T>

class Poligono{
    private:
        // Vector de puntos que forman el poligono
        std::vector<Punto<T>> vertices;
    public:
        // Constructores
        Poligono();
        // Copia
        Poligono(const std::vector<Punto<T>> &v);

        // Funciones
        int size() const;
        bool es_ccw();
        void invertir();
        double area();

        // Overloads
        Punto<T>& operator[](int i);
        const Punto<T>& operator[](int i) const;
        
        // Print
        friend std::ostream &operator<<(std::ostream &os, const Poligono<T> &p){
            os<<"[";
            for(int i = 0; i < p.size(); i++){
                if(i > 0){
                    os<<",";
                }
                os<<p[i];
            }
            os <<"]";
            return os;
        };

        ~Poligono();
};