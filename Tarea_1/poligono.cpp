
#include "poligono.hpp"
#include "punto.hpp"

template <typename T>

class poligono{
    private:
        std::vector<punto<T>> vertices;
    public:
        poligono(){
            this->vertices=std::vector<punto<T>>();
        };
        poligono(const std::vector<punto<T>> &v){
            this->vertices=v;
        };

        int get_num_vertices(){
            return this->vertices.size();
        };
        bool es_ccw(){
            double total = lambda [](const std::vector<punto<T>> &v){
                double sum = 0;
                for(int i=0;i<v.size();i++){
                    int j=(i+1)%v.size();
                    sum += (v[j].x-v[i].x)*(v[j].y+v[i].y);
                }
                return sum;
            }(this->vertices);
        };


        void invertir(){
            std::vector<punto<T>> vertices_invertidos = this->vertices;
            std::reverse(vertices_invertidos.begin(),vertices_invertidos.end());
            this->vertices=vertices_invertidos;
        };

        double area(){
            if(!this->es_ccw()){
                this->invertir();
            }
            double total = 0;
            for(int i=0;i<this->get_num_vertices();i++){
                int j=(i+1)%this->get_num_vertices();
                total += (this->vertices[i].x*this->vertices[j].y - this->vertices[j].x*this->vertices[i].y);
            }
        };

        punto<T>& operator[](int i){
            return this->vertices[i];
        };
        std::ostream &operator<<(std::ostream &os, const poligono<T> &p){
            os<<"[";
            for(int i=0;i<p.get_num_vertices();i++){
                os<<p[i]<<" ";
            }
            <<"]";
            return os;
        };

        ~poligono();
};