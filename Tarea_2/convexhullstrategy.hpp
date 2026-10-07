#pragma once

#include "punto.hpp"
#include "poligono.hpp"
#include <vector>

template <typename T>
class ConvexHullStrategy {
public:
    virtual ~ConvexHullStrategy() = default;
    virtual Poligono<T> apply(const std::vector<Punto<T>>& cloud) const = 0;
};