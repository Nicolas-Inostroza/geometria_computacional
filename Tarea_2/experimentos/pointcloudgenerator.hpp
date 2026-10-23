#pragma once

#include "../tarea1/punto.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

enum class BoundaryShape {
    CollinearSides,
    StrictlyConvex
};

template <typename T>
class PointCloudGenerator {
public:
    virtual ~PointCloudGenerator() = default;
    virtual std::vector<Punto<T>> generate(
        std::size_t count,
        std::mt19937& generator
    ) const = 0;
    virtual std::string name() const = 0;
    virtual double boundary_percentage() const = 0;
};

template <typename T>
class UniformPointCloudGenerator : public PointCloudGenerator<T> {
public:
    std::vector<Punto<T>> generate(
        std::size_t count,
        std::mt19937& generator
    ) const override {
        std::vector<Punto<T>> cloud;
        cloud.reserve(count);

        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<long long> coordinate(-1000000, 1000000);
            for (std::size_t i = 0; i < count; ++i) {
                cloud.emplace_back(
                    static_cast<T>(coordinate(generator)),
                    static_cast<T>(coordinate(generator))
                );
            }
        } else {
            std::uniform_real_distribution<double> coordinate(-1.0, 1.0);
            for (std::size_t i = 0; i < count; ++i) {
                cloud.emplace_back(
                    static_cast<T>(coordinate(generator)),
                    static_cast<T>(coordinate(generator))
                );
            }
        }

        return cloud;
    }

    std::string name() const override {
        return std::is_integral<T>::value ? "uniforme_entero" : "uniforme_real";
    }

    double boundary_percentage() const override {
        return 0.0;
    }
};

template <typename T>
class BoundaryInteriorPointCloudGenerator : public PointCloudGenerator<T> {
private:
    double percentage;
    BoundaryShape shape;

    static T convert_coordinate(double value) {
        if constexpr (std::is_integral<T>::value) {
            return static_cast<T>(std::llround(value));
        } else {
            return static_cast<T>(value);
        }
    }

    void append_collinear_boundary(
        std::vector<Punto<T>>& cloud,
        std::size_t boundary_count
    ) const {
        const double extent = std::is_integral<T>::value
            ? static_cast<double>(boundary_count + 1)
            : 1.0;

        for (std::size_t i = 0; i < boundary_count; ++i) {
            const double position =
                4.0 * static_cast<double>(i) / boundary_count;
            const int side = static_cast<int>(position);
            const double fraction = position - side;
            const double coordinate = (2.0 * fraction - 1.0) * extent;

            switch (side) {
                case 0:
                    cloud.emplace_back(
                        convert_coordinate(-extent),
                        convert_coordinate(coordinate)
                    );
                    break;
                case 1:
                    cloud.emplace_back(
                        convert_coordinate(coordinate),
                        convert_coordinate(extent)
                    );
                    break;
                case 2:
                    cloud.emplace_back(
                        convert_coordinate(extent),
                        convert_coordinate(-coordinate)
                    );
                    break;
                default:
                    cloud.emplace_back(
                        convert_coordinate(-coordinate),
                        convert_coordinate(-extent)
                    );
                    break;
            }
        }
    }

    void append_strictly_convex_boundary(
        std::vector<Punto<T>>& cloud,
        std::size_t boundary_count,
        std::mt19937& generator
    ) const {
        if constexpr (std::is_integral<T>::value) {
            for (std::size_t i = 0; i < boundary_count; ++i) {
                const long long x = static_cast<long long>(i);
                cloud.emplace_back(static_cast<T>(x), static_cast<T>(x * x));
            }
        } else {
            const double pi = std::acos(-1.0);
            std::uniform_real_distribution<double> rotation_distribution(
                0.0, 2.0 * pi
            );
            const double rotation = rotation_distribution(generator);
            for (std::size_t i = 0; i < boundary_count; ++i) {
                const double angle = rotation +
                    2.0 * pi * static_cast<double>(i) / boundary_count;
                cloud.emplace_back(
                    static_cast<T>(std::cos(angle)),
                    static_cast<T>(std::sin(angle))
                );
            }
        }
    }

    void append_interior_points(
        std::vector<Punto<T>>& cloud,
        std::size_t boundary_count,
        std::size_t interior_count,
        std::mt19937& generator
    ) const {
        if (interior_count == 0) {
            return;
        }

        if (shape == BoundaryShape::StrictlyConvex &&
            std::is_integral<T>::value) {
            if (boundary_count < 4) {
                throw std::invalid_argument(
                    "La frontera entera estrictamente convexa requiere "
                    "al menos cuatro puntos"
                );
            }

            const long long last_x =
                static_cast<long long>(boundary_count - 1);
            std::uniform_int_distribution<long long> x_distribution(
                1, last_x - 1
            );
            for (std::size_t i = 0; i < interior_count; ++i) {
                const long long x = x_distribution(generator);
                const long long lower = x * x;
                const long long upper = last_x * x;
                std::uniform_int_distribution<long long> y_distribution(
                    lower + 1, upper - 1
                );
                cloud.emplace_back(
                    static_cast<T>(x),
                    static_cast<T>(y_distribution(generator))
                );
            }
            return;
        }

        if (shape == BoundaryShape::StrictlyConvex) {
            const double pi = std::acos(-1.0);
            std::uniform_real_distribution<double> angle_distribution(
                0.0, 2.0 * pi
            );
            std::uniform_real_distribution<double> radius_distribution(0.0, 1.0);
            for (std::size_t i = 0; i < interior_count; ++i) {
                const double angle = angle_distribution(generator);
                const double radius =
                    0.8 * std::sqrt(radius_distribution(generator));
                cloud.emplace_back(
                    static_cast<T>(radius * std::cos(angle)),
                    static_cast<T>(radius * std::sin(angle))
                );
            }
            return;
        }

        const double extent = std::is_integral<T>::value
            ? static_cast<double>(boundary_count + 1)
            : 1.0;
        std::uniform_real_distribution<double> coordinate(-0.5 * extent, 0.5 * extent);
        for (std::size_t i = 0; i < interior_count; ++i) {
            cloud.emplace_back(
                convert_coordinate(coordinate(generator)),
                convert_coordinate(coordinate(generator))
            );
        }
    }

public:
    BoundaryInteriorPointCloudGenerator(
        double boundary_percentage,
        BoundaryShape boundary_shape
    ) : percentage(boundary_percentage), shape(boundary_shape) {
        if (percentage <= 0.0 || percentage > 100.0) {
            throw std::invalid_argument(
                "El porcentaje de frontera debe estar en (0, 100]"
            );
        }
    }

    std::vector<Punto<T>> generate(
        std::size_t count,
        std::mt19937& generator
    ) const override {
        const std::size_t boundary_count = static_cast<std::size_t>(
            std::llround(count * percentage / 100.0)
        );
        const std::size_t interior_count = count - boundary_count;
        std::vector<Punto<T>> cloud;
        cloud.reserve(count);

        if (shape == BoundaryShape::CollinearSides) {
            append_collinear_boundary(cloud, boundary_count);
        } else {
            append_strictly_convex_boundary(cloud, boundary_count, generator);
        }
        append_interior_points(
            cloud,
            boundary_count,
            interior_count,
            generator
        );
        return cloud;
    }

    std::string name() const override {
        const char* coordinate_type =
            std::is_integral<T>::value ? "entero" : "real";
        const char* shape_name = shape == BoundaryShape::CollinearSides
            ? "lados_colineales"
            : "frontera_estrictamente_convexa";
        return std::string(coordinate_type) + "_" + shape_name;
    }

    double boundary_percentage() const override {
        return percentage;
    }
};
