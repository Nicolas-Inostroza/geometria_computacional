#pragma once

#include "convexhullstrategy.hpp"
#include "vector.hpp"
#include <algorithm>
#include <random>
#include <utility>
#include <random>
#include <utility>
#include <vector>

template <typename T>
class RandomIncrementalAlgorithm : public ConvexHullStrategy<T> {
private:
    // Compara puntos en el plano de XY.
    static bool same_xy(const Punto<T>& a, const Punto<T>& b) {
        return a.get_x() == b.get_x() && a.get_y() == b.get_y();
    }

    // Calcula la orientación de a, b y c mediante el producto cruzado 2D. El signo indica si c está a la izquierda, a la derecha o sobre ab.
    static T cross(
        const Punto<T>& a,
        const Punto<T>& b,
        const Punto<T>& c
    ) {
        const T abx = b.get_x() - a.get_x();
        const T aby = b.get_y() - a.get_y();
        const T acx = c.get_x() - a.get_x();
        const T acy = c.get_y() - a.get_y();

        Vector<T> ab(abx, aby);
        Vector<T> ac(acx, acy);
        Vector<T> cross_product = ab.producto_cruz(ac);
        return cross_product.get_origen().get_z();
    }

    // Incorpora un punto a una cápsula convexa cuyos vértices están en orden
    // antihorario. Si el punto está dentro o sobre el borde, no cambia la cápsula.
    static void insert_point(
        std::vector<Punto<T>>& hull,
        const Punto<T>& point
    ) {
        const std::size_t size = hull.size();
        std::vector<T> turns(size);
        bool outside = false;

        for (std::size_t i = 0; i < size; ++i) {
            // En orientación antihoraria, un valor negativo indica una arista
            // desde la que el punto queda fuera y que, por tanto, es visible.
            turns[i] = cross(hull[i], hull[(i + 1) % size], point);
            outside = outside || turns[i] < 0;
        }

        if (!outside) {
            return;
        }

        // Localiza el inicio del tramo continuo de aristas visibles.
        std::size_t start = 0;
        while (!(turns[start] <= 0 && turns[(start + size - 1) % size] > 0)) {
            ++start;
        }

        // Avanza hasta la última arista visible del tramo.
        std::size_t end = start;
        while (turns[(end + 1) % size] <= 0) {
            end = (end + 1) % size;
        }

        // Conserva el arco no visible y reemplaza el arco visible por dos
        // nuevas aristas que conectan sus extremos con el punto insertado.
        std::vector<Punto<T>> updated_hull;
        updated_hull.reserve(size + 1);
        updated_hull.push_back(hull[start]);
        updated_hull.push_back(point);

        // Copia los vértices que quedan fuera de la cadena visible.
        std::size_t i = (end + 1) % size;
        while (i != start) {
            updated_hull.push_back(hull[i]);
            i = (i + 1) % size;
        }

        hull = std::move(updated_hull);
    }

public:
    Poligono<T> apply(const std::vector<Punto<T>>& cloud) const override {
        if (cloud.empty()) {
            return Poligono<T>();
        }

        // Una permutación aleatoria define el orden de inserción incremental.
        std::vector<Punto<T>> points = cloud;
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::shuffle(points.begin(), points.end(), generator);

        // Busca dos puntos distintos en XY y un tercero no colineal.
        const Punto<T>& first = points.front();
        std::size_t second_index = 1;
        while (second_index < points.size() &&
               same_xy(first, points[second_index])) {
            ++second_index;
        }

        if (second_index == points.size()) {
            return Poligono<T>({first});
        }

        // Busca un tercer punto que no esté sobre la recta de los dos primeros.
        std::size_t third_index = second_index + 1;
        while (third_index < points.size() &&
               cross(first, points[second_index], points[third_index]) == 0) {
            ++third_index;
        }

        if (third_index == points.size()) {
            // Si todos son colineales, la cerradura son los dos extremos.
            auto less_xy = [](const Punto<T>& a, const Punto<T>& b) {
                if (a.get_x() != b.get_x()) {
                    return a.get_x() < b.get_x();
                }
                return a.get_y() < b.get_y();
            };
            const auto extremes = std::minmax_element(
                points.begin(), points.end(), less_xy
            );
            if (same_xy(*extremes.first, *extremes.second)) {
                return Poligono<T>({*extremes.first});
            }
            return Poligono<T>({*extremes.first, *extremes.second});
        }

        // Estos tres puntos forman una cápsula triangular inicial.
        std::vector<Punto<T>> hull = {
            points[second_index],
            points[third_index],
            first
        };
        // Normaliza la orientación para que las aristas visibles se detecten
        // consistentemente en insert_point.
        if (cross(hull[0], hull[1], hull[2]) < 0) {
            std::swap(hull[1], hull[2]);
        }

        // Inserta los puntos restantes; los duplicados del triángulo inicial
        // se omiten porque ya están representados en la cápsula.
        for (const Punto<T>& point : points) {
            if (same_xy(point, points[second_index]) ||
                same_xy(point, points[third_index]) ||
                same_xy(point, first)) {
                continue;
            }
            insert_point(hull, point);
        }

        return Poligono<T>(hull);
    }
};