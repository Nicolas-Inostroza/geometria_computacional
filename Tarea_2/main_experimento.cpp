#include "punto.cpp"
#include "vector.cpp"
#include "poligono.cpp"
#include "incrementalalgorithm.hpp"
#include "randomincrementalalgorithm.hpp"
#include "pointcloudgenerator.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;
constexpr int first_exponent = 4;
constexpr int default_max_exponent = 6;
constexpr int default_repetitions = 5;

template <typename T>
using Cloud = std::vector<Punto<T>>;

template <typename T>
bool same_hull_vertices(const Poligono<T>& first, const Poligono<T>& second) {
    if (first.size() != second.size()) {
        return false;
    }

    std::vector<std::pair<T, T>> first_vertices;
    std::vector<std::pair<T, T>> second_vertices;
    first_vertices.reserve(first.size());
    second_vertices.reserve(second.size());

    for (int i = 0; i < first.size(); ++i) {
        first_vertices.emplace_back(first[i].get_x(), first[i].get_y());
        second_vertices.emplace_back(second[i].get_x(), second[i].get_y());
    }

    const auto by_coordinates = [](const auto& a, const auto& b) {
        if (a.first != b.first) {
            return a.first < b.first;
        }
        return a.second < b.second;
    };
    std::sort(first_vertices.begin(), first_vertices.end(), by_coordinates);
    std::sort(second_vertices.begin(), second_vertices.end(), by_coordinates);
    return first_vertices == second_vertices;
}

double average_milliseconds(const std::vector<double>& measurements) {
    double total = 0.0;
    for (double measurement : measurements) {
        total += measurement;
    }
    return total / measurements.size();
}

template <typename T, typename Generator>
void run_scenario(
    const Generator& cloud_generator,
    std::size_t count,
    int repetitions,
    std::mt19937& generator,
    std::ofstream& csv
) {
    IncrementalAlgorithm<T> incremental;
    std::vector<double> incremental_times;
    std::vector<double> random_incremental_times;
    int hull_vertices = 0;

    for (int repetition = 0; repetition < repetitions; ++repetition) {
        const Cloud<T> cloud = cloud_generator.generate(count, generator);
        const unsigned int algorithm_seed = generator();

        const auto incremental_start = Clock::now();
        const Poligono<T> incremental_hull = incremental.apply(cloud);
        const auto incremental_end = Clock::now();

        RandomIncrementalAlgorithm<T> random_incremental(algorithm_seed);
        const auto random_start = Clock::now();
        const Poligono<T> random_hull = random_incremental.apply(cloud);
        const auto random_end = Clock::now();

        if (!same_hull_vertices(incremental_hull, random_hull)) {
            throw std::runtime_error(
                "Los algoritmos produjeron cerraduras distintas en " +
                cloud_generator.name() + ", n=" + std::to_string(count)
            );
        }

        incremental_times.push_back(
            std::chrono::duration<double, std::milli>(
                incremental_end - incremental_start
            ).count()
        );
        random_incremental_times.push_back(
            std::chrono::duration<double, std::milli>(
                random_end - random_start
            ).count()
        );
        hull_vertices = incremental_hull.size();
    }

    const double incremental_average = average_milliseconds(incremental_times);
    const double random_average = average_milliseconds(random_incremental_times);
    const std::string scenario = cloud_generator.name();
    const double boundary_percentage = cloud_generator.boundary_percentage();

    csv << scenario << ','
        << count << ','
        << boundary_percentage << ','
        << repetitions << ','
        << incremental_average << ','
        << random_average << ','
        << hull_vertices << ",true\n";

    std::cout << scenario << ", n=" << count
              << ", borde=" << boundary_percentage << "%"
              << ", hull=" << hull_vertices
              << ", incremental=" << incremental_average << " ms"
              << ", random_incremental=" << random_average << " ms\n";
}

int parse_positive_integer(const char* value, const std::string& argument_name) {
    std::size_t parsed_length = 0;
    const std::string text(value);
    const int parsed = std::stoi(text, &parsed_length);
    if (parsed_length != text.size() || parsed <= 0) {
        throw std::invalid_argument(argument_name + " debe ser un entero positivo");
    }
    return parsed;
}

template <typename T>
void run_for_coordinate_type(
    std::size_t count,
    int repetitions,
    std::mt19937& generator,
    std::ofstream& csv
) {
    const UniformPointCloudGenerator<T> uniform;
    run_scenario<T>(uniform, count, repetitions, generator, csv);

    std::vector<double> percentages;
    if (count == 10000) {
        percentages = {0.1, 0.15, 0.2, 0.3, 0.5, 1.0, 10.0, 70.0};
    } else if (count == 100000) {
        percentages = {0.1, 1.0};
    } else {
        percentages = {0.01};
    }

    for (double percentage : percentages) {
        const BoundaryInteriorPointCloudGenerator<T> strict(
            percentage, BoundaryShape::StrictlyConvex
        );
        run_scenario<T>(strict, count, repetitions, generator, csv);
    }

    if (count == 10000) {
        for (double percentage : {10.0, 70.0}) {
            const BoundaryInteriorPointCloudGenerator<T> collinear(
                percentage, BoundaryShape::CollinearSides
            );
            run_scenario<T>(collinear, count, repetitions, generator, csv);
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc > 5) {
            throw std::invalid_argument(
                "Uso: main_experimento [exponente_maximo] [repeticiones] "
                "[archivo.csv] [semilla]"
            );
        }

        const int max_exponent = argc > 1
            ? parse_positive_integer(argv[1], "exponente_maximo")
            : default_max_exponent;
        const int repetitions = argc > 2
            ? parse_positive_integer(argv[2], "repeticiones")
            : default_repetitions;
        const std::string output_path = argc > 3
            ? argv[3]
            : "resultados_experimento.csv";
        const unsigned int seed = argc > 4
            ? static_cast<unsigned int>(
                parse_positive_integer(argv[4], "semilla")
            )
            : 20261022U;

        if (max_exponent < first_exponent || max_exponent > 6) {
            throw std::invalid_argument(
                "El exponente_maximo debe estar entre 4 y 6"
            );
        }

        std::ofstream csv(output_path);
        if (!csv) {
            throw std::runtime_error("No se pudo crear " + output_path);
        }
        csv << "escenario,n,porcentaje_puntos_en_borde,repeticiones,"
               "incremental_ms_promedio,random_incremental_ms_promedio,"
               "vertices_cerradura,cerraduras_iguales\n";

        std::mt19937 generator(seed);
        std::cout << "Semilla: " << seed << "\n"
                  << "Resultados CSV: " << output_path << "\n"
                  << "Se comparan tipos enteros y reales, varios porcentajes "
                     "y fronteras colineales/estrictamente convexas.\n";

        for (int exponent = first_exponent;
             exponent <= max_exponent;
             ++exponent) {
            std::size_t points = 1;
            for (int i = 0; i < exponent; ++i) {
                points *= 10;
            }

            run_for_coordinate_type<double>(
                points, repetitions, generator, csv
            );
            run_for_coordinate_type<long long>(
                points, repetitions, generator, csv
            );
        }

        std::cout << "Experimentos terminados. Todas las cerraduras coincidieron.\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
