#include "punto.hpp"
#include <ostream>
#include <vector>
#pragma once

template <typename T>

class Poligono{
    private:
        std::vector<Punto<T>> vertices;
    public:
        Poligono();
        Poligono(const std::vector<Punto<T>> &v);

        int size() const;
        bool es_ccw();
        void invertir();
        double area();

        Punto<T>& operator[](int i);
        const Punto<T>& operator[](int i) const;
        

        friend std::ostream &operator<<(std::ostream &os, const Poligono<T> &p){
            os<<"[";
            for(int i=0;i<p.size();i++){
                os<<p[i]<<" ";
            }
            os <<"]";
            return os;
        };

        ~Poligono();
};