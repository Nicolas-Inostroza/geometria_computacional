#include "punto.hpp"

template <typename T>

class poligono{
    private:
        std::vector<punto<T>> vertices;
    public:
        poligono();
        poligono(const std::vector<punto<T>> &v);

        int get_num_vertices();
        bool es_ccw();
        void invertir();
        double area();

        punto<T>& operator[](int i);
        std::ostream &operator<<(std::ostream &os, const poligono<T> &p);

        ~poligono();
};