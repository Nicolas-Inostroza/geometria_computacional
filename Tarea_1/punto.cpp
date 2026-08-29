#include "punto.hpp"

template <typename T>

class punto{
    private:
        T x;
        T y;
        T z;
    public:
        punto(){
            this->x=0;
            this->y=0;
            this->z=0;
        };
        punto(T x_valor,T y_valor){
            this->x=x_valor;
            this->y=y_valor;
            this->z=0;
        };
        punto(const punto<T> &p){
            this->x=p.x;
            this->y=p.y;
            this->z=p.z;
        };

        bool operator==(const punto<T> &p){
            return (this->x==p.x && this->y==p.y && this->z==p.z);
        };
        std::ostream &operator<<(std::ostream &os,const punto<T> &p){
            os<<"("<<p.x<<","<<p.y<<","<<p.z<<")";
            return os;
        };
        ~punto(){
            free(this);
        };
};
