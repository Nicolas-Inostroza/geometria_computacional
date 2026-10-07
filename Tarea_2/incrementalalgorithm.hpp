#pragma once

#include "convexhullstrategy.hpp"
#include <algorithm>
#include <vector>

template <typename T>
class IncrementalAlgorithm : public ConvexHullStrategy<T> {
private:
    static long double cross(
        const Punto<T>& a,
        const Punto<T>& b,
        const Punto<T>& c
    ) {
        const long double abx =
            static_cast<long double>(b.get_x()) - a.get_x();
        const long double aby =
            static_cast<long double>(b.get_y()) - a.get_y();
        const long double acx =
            static_cast<long double>(c.get_x()) - a.get_x();
        const long double acy =
            static_cast<long double>(c.get_y()) - a.get_y();

        return abx * acy - aby * acx;
    }

public:
    Poligono<T> apply(const std::vector<Punto<T>>& cloud) const override {
        std::vector<Punto<T>> points = cloud;

        std::sort(points.begin(), points.end(),
            [](const Punto<T>& a, const Punto<T>& b) {
                if (a.get_x() != b.get_x())
                    return a.get_x() < b.get_x();
                return a.get_y() < b.get_y();
            });

        // Quita puntos repetidos en el plano XY.
        points.erase(
            std::unique(points.begin(), points.end(),
                [](const Punto<T>& a, const Punto<T>& b) {
                    return a.get_x() == b.get_x()
                        && a.get_y() == b.get_y();
                }),
            points.end()
        );

        if (points.size() <= 2)
            return Poligono<T>(points);

        std::vector<Punto<T>> lower;
        for (const Punto<T>& p : points) {
            while (lower.size() >= 2 &&
                   cross(lower[lower.size() - 2], lower.back(), p) <= 0) {
                lower.pop_back();
            }
            lower.push_back(p);
        }

        std::vector<Punto<T>> upper;
        for (auto it = points.rbegin(); it != points.rend(); ++it) {
            while (upper.size() >= 2 &&
                   cross(upper[upper.size() - 2], upper.back(), *it) <= 0) {
                upper.pop_back();
            }
            upper.push_back(*it);
        }

        // Los extremos aparecen en ambas listas; se quitan antes de unirlas.
        lower.pop_back();
        upper.pop_back();
        lower.insert(lower.end(), upper.begin(), upper.end());

        return Poligono<T>(lower);
    }
};
