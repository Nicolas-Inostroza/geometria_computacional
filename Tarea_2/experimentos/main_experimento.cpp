#include "../tarea1/punto.cpp"
#include "../tarea1/vector.cpp"
#include "../tarea1/poligono.cpp"
#include "../algorithmos/incrementalalgorithm.hpp"
#include "../algorithmos/randomincrementalalgorithm.hpp"
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

using Reloj = std::chrono::steady_clock;
constexpr int exponente_inicial = 4;
constexpr int exponente_maximo_predeterminado = 7;
constexpr int repeticiones_predeterminadas = 5;

template <typename T>
using Nube = std::vector<Punto<T>>;

template <typename T>
bool mismos_vertices_cerradura(
    const Poligono<T>& poligono_primero,
    const Poligono<T>& poligono_segundo
) {
    // Se comparan los conjuntos de vértices, no el orden en que vienen listados.
    if (poligono_primero.size() != poligono_segundo.size()) {
        return false;
    }

    std::vector<std::pair<T, T>> vertices_primer_poligono;
    std::vector<std::pair<T, T>> vertices_segundo_poligono;
    vertices_primer_poligono.reserve(poligono_primero.size());
    vertices_segundo_poligono.reserve(poligono_segundo.size());

    for (int indice = 0; indice < poligono_primero.size(); ++indice) {
        vertices_primer_poligono.emplace_back(
            poligono_primero[indice].get_x(), poligono_primero[indice].get_y()
        );
        vertices_segundo_poligono.emplace_back(
            poligono_segundo[indice].get_x(), poligono_segundo[indice].get_y()
        );
    }

    const auto por_coordenada = [](const auto& coordenada_a, const auto& coordenada_b) {
        if (coordenada_a.first != coordenada_b.first) {
            return coordenada_a.first < coordenada_b.first;
        }
        return coordenada_a.second < coordenada_b.second;
    };
    // Ordenar permite comparar las cerraduras sin depender de su orientación.
    std::sort(
        vertices_primer_poligono.begin(), vertices_primer_poligono.end(),
        por_coordenada
    );
    std::sort(
        vertices_segundo_poligono.begin(), vertices_segundo_poligono.end(),
        por_coordenada
    );
    return vertices_primer_poligono == vertices_segundo_poligono;
}

// Metodo auxiliar para calcular el promedio de tiempo.
double promedio_milisegundos(const std::vector<double>& mediciones) {
    double total = 0.0;
    for (double medicion : mediciones) {
        total += medicion;
    }
    return total / mediciones.size();
}

template <typename T, typename Generator>
void ejecutar_escenario(
    const Generator& generador_nube,
    std::size_t cantidad_puntos,
    int repeticiones,
    std::mt19937& generador_aleatorio,
    std::ofstream& archivo_csv
) {
    // Cada repetición mide ambos algoritmos sobre la misma nube de puntos.
    IncrementalAlgorithm<T> algoritmo_incremental;
    std::vector<double> tiempos_incremental;
    std::vector<double> tiempos_incremental_aleatorio;
    int cantidad_vertices_cerradura = 0;

    for (int repeticion = 0; repeticion < repeticiones; ++repeticion) {
        const Nube<T> nube =
            generador_nube.generar(cantidad_puntos, generador_aleatorio);
        const unsigned int semilla_algoritmo = generador_aleatorio();

        const auto inicio_incremental = Reloj::now();
        const Poligono<T> cerradura_incremental =
            algoritmo_incremental.apply(nube);
        const auto fin_incremental = Reloj::now();

        RandomIncrementalAlgorithm<T> algoritmo_incremental_aleatorio(
            semilla_algoritmo
        );
        const auto inicio_incremental_aleatorio = Reloj::now();
        const Poligono<T> cerradura_incremental_aleatoria =
            algoritmo_incremental_aleatorio.apply(nube);
        const auto fin_incremental_aleatorio = Reloj::now();

        if (!mismos_vertices_cerradura(
                cerradura_incremental, cerradura_incremental_aleatoria
            )) {
            throw std::runtime_error(
                "Los algoritmos produjeron cerraduras distintas en " +
                generador_nube.nombre() + ", n=" +
                std::to_string(cantidad_puntos)
            );
        }

        tiempos_incremental.push_back(
            std::chrono::duration<double, std::milli>(
                fin_incremental - inicio_incremental
            ).count()
        );
        tiempos_incremental_aleatorio.push_back(
            std::chrono::duration<double, std::milli>(
                fin_incremental_aleatorio - inicio_incremental_aleatorio
            ).count()
        );
        cantidad_vertices_cerradura = cerradura_incremental.size();
    }

    const double promedio_incremental = promedio_milisegundos(tiempos_incremental);
    const double promedio_incremental_aleatorio =
        promedio_milisegundos(tiempos_incremental_aleatorio);
    const std::string nombre_escenario = generador_nube.nombre();
    const double porcentaje_frontera = generador_nube.porcentaje_frontera();

    archivo_csv << nombre_escenario << ','
        << cantidad_puntos << ','
        << porcentaje_frontera << ','
        << repeticiones << ','
        << promedio_incremental << ','
        << promedio_incremental_aleatorio << ','
        << cantidad_vertices_cerradura << ",true\n";

    std::cout << nombre_escenario << ", n=" << cantidad_puntos
              << ", borde=" << porcentaje_frontera << "%"
              << ", hull=" << cantidad_vertices_cerradura
              << ", incremental=" << promedio_incremental << " ms"
              << ", random_incremental=" << promedio_incremental_aleatorio
              << " ms\n";
}

int analizar_entero_positivo(
    const char* valor,
    const std::string& nombre_argumento
) {
    // Rechaza argumentos parcialmente numéricos, cero y valores negativos.
    std::size_t longitud_analizada = 0;
    const std::string texto(valor);
    const int numero_analizado = std::stoi(texto, &longitud_analizada);
    if (longitud_analizada != texto.size() || numero_analizado <= 0) {
        throw std::invalid_argument(
            nombre_argumento + " debe ser un entero positivo"
        );
    }
    return numero_analizado;
}

template <typename T>
void ejecutar_para_tipo_coordenada(
    std::size_t cantidad_puntos,
    int repeticiones,
    std::mt19937& generador_aleatorio,
    std::ofstream& archivo_csv
) {
    // Ejecuta el caso uniforme y los casos con frontera configurada.
    const UniformPointCloudGenerator<T> generador_uniforme;
    ejecutar_escenario<T>(
        generador_uniforme, cantidad_puntos, repeticiones,
        generador_aleatorio, archivo_csv
    );

    std::vector<double> porcentajes;
    if (cantidad_puntos == 10000) {
        porcentajes = {0.1, 0.15, 0.2, 0.3, 0.5, 1.0, 10.0, 70.0};
    } else if (cantidad_puntos == 100000) {
        porcentajes = {0.1, 1.0};
    } else if (cantidad_puntos == 1000000) {
        porcentajes = {0.01};
    } else {
        porcentajes = {0.001};
    }

    for (double porcentaje : porcentajes) {
        const BoundaryInteriorPointCloudGenerator<T> generador_convexo(
            porcentaje, BoundaryShape::StrictlyConvex
        );
        ejecutar_escenario<T>(
            generador_convexo, cantidad_puntos, repeticiones,
            generador_aleatorio, archivo_csv
        );
    }

    if (cantidad_puntos == 10000) {
        for (double porcentaje : {10.0, 70.0}) {
            const BoundaryInteriorPointCloudGenerator<T> generador_colineal(
                porcentaje, BoundaryShape::CollinearSides
            );
            ejecutar_escenario<T>(
                generador_colineal, cantidad_puntos, repeticiones,
                generador_aleatorio, archivo_csv
            );
        }
    }
}

} // namespace

int main(int cantidad_argumentos, char* argumentos[]) {
    try {
        // Lee parámetros opcionales y prepara el archivo donde se guardarán resultados.
        if (cantidad_argumentos > 5) {
            throw std::invalid_argument(
                "Uso: main_experimento [exponente_maximo] [repeticiones] "
                "[archivo.csv] [semilla]"
            );
        }

        const int exponente_maximo = cantidad_argumentos > 1
            ? analizar_entero_positivo(argumentos[1], "exponente_maximo")
            : exponente_maximo_predeterminado;
        const int repeticiones = cantidad_argumentos > 2
            ? analizar_entero_positivo(argumentos[2], "repeticiones")
            : repeticiones_predeterminadas;
        const std::string ruta_salida = cantidad_argumentos > 3
            ? argumentos[3]
            : "experimentos/resultados_experimento.csv";
        const unsigned int semilla = cantidad_argumentos > 4
            ? static_cast<unsigned int>(
                analizar_entero_positivo(argumentos[4], "semilla")
            )
            : 20261022U;

        if (exponente_maximo < exponente_inicial || exponente_maximo > 7) {
            throw std::invalid_argument(
                "El exponente_maximo debe estar entre 4 y 7"
            );
        }

        std::ofstream archivo_csv(ruta_salida);
        if (!archivo_csv) {
            throw std::runtime_error("No se pudo crear " + ruta_salida);
        }
        archivo_csv << "escenario,n,porcentaje_puntos_en_borde,repeticiones,"
                       "incremental_ms_promedio,random_incremental_ms_promedio,"
                       "vertices_cerradura,cerraduras_iguales\n";

        std::mt19937 generador_aleatorio(semilla);
        std::cout << "Semilla: " << semilla << "\n"
                  << "Resultados CSV: " << ruta_salida << "\n"
                  << "Se comparan tipos enteros y reales, varios porcentajes "
                     "y fronteras colineales/estrictamente convexas.\n";

        // Ejecuta ambos tipos de coordenadas para cada tamaño de nube.
        for (int exponente = exponente_inicial;
             exponente <= exponente_maximo;
             ++exponente) {
            std::size_t cantidad_puntos = 1;
            for (int indice = 0; indice < exponente; ++indice) {
                cantidad_puntos *= 10;
            }

            ejecutar_para_tipo_coordenada<double>(
                cantidad_puntos, repeticiones, generador_aleatorio, archivo_csv
            );
            ejecutar_para_tipo_coordenada<long long>(
                cantidad_puntos, repeticiones, generador_aleatorio, archivo_csv
            );
        }

        std::cout << "Experimentos terminados. Todas las cerraduras coincidieron.\n";
    } catch (const std::exception& excepcion) {
        std::cerr << "Error: " << excepcion.what() << '\n';
        return 1;
    }

    return 0;
}
