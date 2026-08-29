#include "punto.hpp"

template <typename T>

class vector{
    private:
        punto<T> origen;
    public:
        vector();
        vector(T x, T y);
        vector(const punto<T> &p);
        vector(const vector<T> &v);

        double magnitud();

        double producto_punto(const vector<T> &v);
        vector<T> producto_cruz(const vector<T> &v);

        vector<T> operator+(const vector<T> &v);
        bool operator==(const vector<T> &v);
        vector<T> operator*(T &escalar);
        std::ostream &operator<<(std::ostream &os, const vector<T> &v);

        ~vector();
};