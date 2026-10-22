#include "poligono.hpp"
#include "punto.hpp"
#include <vector>
#include <algorithm>


template <typename T>
Poligono<T>::Poligono(){
    this->vertices=std::vector<Punto<T>>();
};

template <typename T>
Poligono<T>::Poligono(const std::vector<Punto<T>> &v) {
    this->vertices=v;
};

template <typename T>
int Poligono<T>::size() const{
    return this->vertices.size();
};


template <typename T>
bool Poligono<T>::es_ccw(){
    double total = [](const std::vector<Punto<T>> &v){
        double sum = 0;
        for(int i = 0; i < (int)v.size(); i++){
            int j = (i+1) % v.size();
            sum += (v[j].get_x() - v[i].get_x()) * (v[j].get_y() + v[i].get_y());
        }
        return sum;
    }(this->vertices);
    bool result = total < 0;
    return result;
};


template <typename T>
void Poligono<T>::invertir(){
    std::vector<Punto<T>> vertices_invertidos = this->vertices;
    std::reverse(vertices_invertidos.begin(),vertices_invertidos.end());
    this->vertices=vertices_invertidos;
};

template <typename T>
double Poligono<T>::area(){
    double total = 0;
    for(int i=0;i<this->size();i++){
        int j=(i+1)%this->size();
        total += (this->vertices[i].get_x()*this->vertices[j].get_y() - this->vertices[j].get_x()*this->vertices[i].get_y());
    }
    return std::abs(total)/2.0;
};

template <typename T>
Punto<T>& Poligono<T>::operator[](int i) {
     return this->vertices[i];
}

template <typename T>
const Punto<T>& Poligono<T>::operator[](int i) const {
    return this->vertices[i];
}

template <typename T>
Poligono<T>::~Poligono() = default;