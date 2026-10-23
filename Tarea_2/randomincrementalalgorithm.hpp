#pragma once

#include "convexhullstrategy.hpp"
#include "vector.hpp"
#include <algorithm>
#include <random>
#include <utility>
#include <vector>

template <typename T>
class RandomIncrementalAlgorithm : public ConvexHullStrategy<T> {
private:
    const unsigned int seed;

    static bool same_xy(const Punto<T>& a, const Punto<T>& b) {
        return a.get_x() == b.get_x() && a.get_y() == b.get_y();
    }

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
        return ab.producto_cruz(ac).get_origen().get_z();
    }

    static void insert_point(
        std::vector<Punto<T>>& hull,
        const Punto<T>& point
    ) {
        const std::size_t size = hull.size();
        std::vector<T> turns(size);
        bool outside = false;

        for (std::size_t i = 0; i < size; ++i) {
            turns[i] = cross(hull[i], hull[(i + 1) % size], point);
            outside = outside || turns[i] < 0;
        }

        if (!outside) {
            return;
        }

        // Las aristas visibles forman una cadena continua en una cerradura convexa.
        std::size_t start = 0;
        while (!(turns[start] <= 0 && turns[(start + size - 1) % size] > 0)) {
            ++start;
        }

        std::size_t end = start;
        while (turns[(end + 1) % size] <= 0) {
            end = (end + 1) % size;
        }

        std::vector<Punto<T>> updated_hull;
        updated_hull.reserve(size + 1);
        updated_hull.push_back(hull[start]);
        updated_hull.push_back(point);

        std::size_t i = (end + 1) % size;
        while (i != start) {
            updated_hull.push_back(hull[i]);
            i = (i + 1) % size;
        }

        hull = std::move(updated_hull);
    }

public:
    explicit RandomIncrementalAlgorithm(
        unsigned int random_seed = std::random_device{}()
    ) : seed(random_seed) {}

    Poligono<T> apply(const std::vector<Punto<T>>& cloud) const override {
        if (cloud.empty()) {
            return Poligono<T>();
        }

        // La semilla permite reproducir el orden de insercion en pruebas y experimentos.
        std::vector<Punto<T>> points = cloud;
        std::mt19937 generator(seed);
        std::shuffle(points.begin(), points.end(), generator);

        const Punto<T>& first = points.front();
        std::size_t second_index = 1;
        while (second_index < points.size() &&
               same_xy(first, points[second_index])) {
            ++second_index;
        }

        if (second_index == points.size()) {
            return Poligono<T>({first});
        }

        std::size_t third_index = second_index + 1;
        while (third_index < points.size() &&
               cross(first, points[second_index], points[third_index]) == 0) {
            ++third_index;
        }

        if (third_index == points.size()) {
            // Una nube colineal tiene como cerradura solo sus dos extremos.
            const auto less_xy = [](const Punto<T>& a, const Punto<T>& b) {
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

        std::vector<Punto<T>> hull = {
            points[second_index],
            points[third_index],
            first
        };
        if (cross(hull[0], hull[1], hull[2]) < 0) {
            std::swap(hull[1], hull[2]);
        }

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
