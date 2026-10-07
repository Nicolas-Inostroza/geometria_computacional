#include "punto.cpp"
#include "vector.cpp"
#include "poligono.cpp"
#include "incrementalalgorithm.hpp"
#include "randomincremental.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Point = Punto<double>;
using Cloud = std::vector<Point>;
using Clock = std::chrono::steady_clock;

constexpr int first_exponent = 2;
constexpr int default_max_exponent = 4;
constexpr int default_repetitions = 3;
constexpr double boundary_percentage = 0.70;

Cloud generate_uniform_cloud(std::size_t n, std::mt19937& generator) {
    std::uniform_real_distribution<double> coordinate(-1.0, 1.0);
    Cloud cloud;
    cloud.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        cloud.emplace_back(coordinate(generator), coordinate(generator));
    }
    return cloud;
}

Cloud generate_cloud_with_boundary_points(
    std::size_t n,
    std::mt19937& generator
) {
    const std::size_t boundary_count = static_cast<std::size_t>(
        std::round(n * boundary_percentage)
    );
    const std::size_t interior_count = n - boundary_count;
    const double pi = std::acos(-1.0);
    std::uniform_real_distribution<double> angle_distribution(0.0, 2.0 * pi);
    std::uniform_real_distribution<double> radius_distribution(0.0, 1.0);
    const double rotation = angle_distribution(generator);

    Cloud cloud;
    cloud.reserve(n);

    // Los puntos equiespaciados en el círculo forman la mayoría de la frontera.
    for (std::size_t i = 0; i < boundary_count; ++i) {
        const double angle = rotation + 2.0 * pi * i / boundary_count;
        cloud.emplace_back(std::cos(angle), std::sin(angle));
    }

    // Los restantes se generan dentro de un disco de radio 0.8.
    for (std::size_t i = 0; i < interior_count; ++i) {
        const double angle = angle_distribution(generator);
        const double radius = 0.8 * std::sqrt(radius_distribution(generator));
        cloud.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }

    return cloud;
}

bool same_hull_vertices(const Poligono<double>& first, const Poligono<double>& second) {
    if (first.size() != second.size()) {
        return false;
    }

    std::vector<std::pair<double, double>> first_vertices;
    std::vector<std::pair<double, double>> second_vertices;
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

double average_milliseconds(
    const std::vector<double>& measurements
) {
    double total = 0.0;
    for (double measurement : measurements) {
        total += measurement;
    }
    return total / measurements.size();
}

template <typename Generator>
void run_scenario(
    const std::string& scenario_name,
    std::size_t n,
    int repetitions,
    Generator generate_cloud,
    std::mt19937& generator,
    std::ofstream& csv
) {
    IncrementalAlgorithm<double> incremental;
    RandomIncrementalAlgorithm<double> random_incremental;
    std::vector<double> incremental_times;
    std::vector<double> random_incremental_times;
    int hull_vertices = 0;

    for (int repetition = 0; repetition < repetitions; ++repetition) {
        const Cloud cloud = generate_cloud(n, generator);

        const auto incremental_start = Clock::now();
        const Poligono<double> incremental_hull = incremental.apply(cloud);
        const auto incremental_end = Clock::now();

        const auto random_start = Clock::now();
        const Poligono<double> random_hull = random_incremental.apply(cloud);
        const auto random_end = Clock::now();

        if (!same_hull_vertices(incremental_hull, random_hull)) {
            throw std::runtime_error(
                "Los algoritmos produjeron cerraduras distintas en " +
                scenario_name + ", n=" + std::to_string(n)
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

    const double percentage = scenario_name == "mayoria_en_cerradura"
        ? 100.0 * boundary_percentage
        : 0.0;
    const double incremental_average = average_milliseconds(incremental_times);
    const double random_average = average_milliseconds(random_incremental_times);

    csv << scenario_name << ','
        << n << ','
        << percentage << ','
        << repetitions << ','
        << incremental_average << ','
        << random_average << ','
        << hull_vertices << ",true\n";

    std::cout << scenario_name << ", n=" << n
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
            ? static_cast<unsigned int>(parse_positive_integer(argv[4], "semilla"))
            : 20261007U;

        if (max_exponent < first_exponent || max_exponent > 7) {
            throw std::invalid_argument(
                "El exponente_maximo debe estar entre 2 y 7"
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
                  << "Se verifica que las dos cerraduras sean iguales en cada repeticion.\n";

        for (int exponent = first_exponent; exponent <= max_exponent; ++exponent) {
            const std::size_t n = static_cast<std::size_t>(
                std::pow(10.0, exponent)
            );

            run_scenario(
                "aleatoria_uniforme",
                n,
                repetitions,
                generate_uniform_cloud,
                generator,
                csv
            );
            run_scenario(
                "mayoria_en_cerradura",
                n,
                repetitions,
                generate_cloud_with_boundary_points,
                generator,
                csv
            );
        }

        std::cout << "Experimentos terminados. Cerraduras iguales en todos los casos.\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
