#include "convexhullstrategy.hpp"


template <typename T>
class RandomIncrementalAlgorithm : public ConvexHullStrategy<T> {
public:
    Poligono<T> apply(const std::vector<Punto<T>>& cloud) const override {
        // algoritmo random incremental
    }
};