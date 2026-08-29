#include "punto.hpp"
#include "vector.hpp"

template <typename T>

class vector{
    private:
        punto<T> origen;
    public:
        vector(){
            this->origen=punto<T>();
        };
        vector(T x, T y){
            this->origen=punto<T>(x,y);
        };
        vector(const punto<T> &p){
            this->origen=p;
        };
        vector(const vector<T> &v){
            this->origen=v.origen;
        };

        double magnitud(){
            double mag = sqrt(pow(this->origen.x,2)+pow(this->origen.y,2)+pow(this->origen.z,2));
            return mag;
        };

        double producto_punto(const vector<T> &v){
            double prod = this->origen.x*v.origen.x + this->origen.y*v.origen.y + this->origen.z*v.origen.z;
            return prod;
        };
        vector<T> producto_cruz(const vector<T> &v){
            double x_resultado = this->origen.y*v.origen.z - this->origen.z*v.origen.y;
            double y_resultado = this->origen.z*v.origen.x - this->origen.x*v.origen.z;
            double z_resultado = this->origen.x*v.origen.y - this->origen.y*v.origen.x;
            return vector<T>(x_resultado,y_resultado,z_resultado);
        };

        vector<T> operator+(const vector<T> &v){
            return vector<T>(this->x+v.origen.x,this->y+v.origen.y,this->z+v.origen.z);
        };
        bool operator==(const vector<T> &v){
            return (this->origen==v.origen);
        };
        vector<T> operator*(T &escalar){
            this->origen.x*=escalar
            this->origen.y*=escalar
            this->origen.z*=escalar
            return *this;
        };
        std::ostream &operator<<(std::ostream &os, const vector<T> &v){
            os<<"("<<v.origen.x<<","<<v.origen.y<<","<<v.origen.z<<")";
            return os;
        };

        ~vector(){
            ~origen;
            ~this;
        };
};