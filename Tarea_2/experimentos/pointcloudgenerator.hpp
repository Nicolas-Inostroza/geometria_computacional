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
    // Interfaz común para generar nubes y describir el escenario utilizado.
    virtual std::vector<Punto<T>> generar(
        std::size_t cantidad_puntos,
        std::mt19937& generador
    ) const = 0;
    virtual std::string nombre() const = 0;
    virtual double porcentaje_frontera() const = 0;
};

template <typename T>
class UniformPointCloudGenerator : public PointCloudGenerator<T> {
public:
    // Genera puntos independientes y uniformes en un cuadrado.
    std::vector<Punto<T>> generar(
        std::size_t cantidad_puntos,
        std::mt19937& generador
    ) const override {
        std::vector<Punto<T>> nube;
        nube.reserve(cantidad_puntos);

        // El rango depende del tipo para conservar precisión en reales.
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<long long> distribucion_coordenadas(
                -1000000, 1000000
            );
            for (std::size_t indice = 0; indice < cantidad_puntos; ++indice) {
                nube.emplace_back(
                    static_cast<T>(distribucion_coordenadas(generador)),
                    static_cast<T>(distribucion_coordenadas(generador))
                );
            }
        } else {
            std::uniform_real_distribution<double> distribucion_coordenadas(
                -1.0, 1.0
            );
            for (std::size_t indice = 0; indice < cantidad_puntos; ++indice) {
                nube.emplace_back(
                    static_cast<T>(distribucion_coordenadas(generador)),
                    static_cast<T>(distribucion_coordenadas(generador))
                );
            }
        }

        return nube;
    }

    std::string nombre() const override {
        return std::is_integral<T>::value ? "uniforme_entero" : "uniforme_real";
    }

    double porcentaje_frontera() const override {
        return 0.0;
    }
};

template <typename T>
class BoundaryInteriorPointCloudGenerator : public PointCloudGenerator<T> {
private:
    double porcentaje;
    BoundaryShape forma_frontera;

    static T convertir_coordenada(double valor) {
        if constexpr (std::is_integral<T>::value) {
            return static_cast<T>(std::llround(valor));
        } else {
            return static_cast<T>(valor);
        }
    }

    void agregar_frontera_lados_colineales(
        std::vector<Punto<T>>& nube,
        std::size_t cantidad_frontera
    ) const {
        const double extension = std::is_integral<T>::value
            ? static_cast<double>(cantidad_frontera + 1)
            : 1.0;

        // Distribuye los puntos de frontera a lo largo de los cuatro lados.
        for (std::size_t indice = 0; indice < cantidad_frontera; ++indice) {
            const double posicion =
                4.0 * static_cast<double>(indice) / cantidad_frontera;
            const int lado = static_cast<int>(posicion);
            const double fraccion = posicion - lado;
            const double coordenada = (2.0 * fraccion - 1.0) * extension;

            switch (lado) {
                case 0:
                    nube.emplace_back(
                        convertir_coordenada(-extension),
                        convertir_coordenada(coordenada)
                    );
                    break;
                case 1:
                    nube.emplace_back(
                        convertir_coordenada(coordenada),
                        convertir_coordenada(extension)
                    );
                    break;
                case 2:
                    nube.emplace_back(
                        convertir_coordenada(extension),
                        convertir_coordenada(-coordenada)
                    );
                    break;
                default:
                    nube.emplace_back(
                        convertir_coordenada(-coordenada),
                        convertir_coordenada(-extension)
                    );
                    break;
            }
        }
    }

    void agregar_frontera_estrictamente_convexa(
        std::vector<Punto<T>>& nube,
        std::size_t cantidad_frontera,
        std::mt19937& generador
    ) const {
        if constexpr (std::is_integral<T>::value) {
            // La parábola discreta produce una frontera estrictamente convexa.
            for (std::size_t indice = 0; indice < cantidad_frontera; ++indice) {
                const long long coordenada_x = static_cast<long long>(indice);
                nube.emplace_back(
                    static_cast<T>(coordenada_x),
                    static_cast<T>(coordenada_x * coordenada_x)
                );
            }
        } else {
            const double pi = std::acos(-1.0);
            std::uniform_real_distribution<double> distribucion_rotacion(
                0.0, 2.0 * pi
            );
            const double rotacion = distribucion_rotacion(generador);
            for (std::size_t indice = 0; indice < cantidad_frontera; ++indice) {
                const double angulo = rotacion +
                    2.0 * pi * static_cast<double>(indice) / cantidad_frontera;
                nube.emplace_back(
                    static_cast<T>(std::cos(angulo)),
                    static_cast<T>(std::sin(angulo))
                );
            }
        }
    }

    void agregar_puntos_interiores(
        std::vector<Punto<T>>& nube,
        std::size_t cantidad_frontera,
        std::size_t cantidad_interior,
        std::mt19937& generador
    ) const {
        if (cantidad_interior == 0) {
            return;
        }

        // La distribución interior depende de la forma y del tipo de frontera.
        if (forma_frontera == BoundaryShape::StrictlyConvex &&
            std::is_integral<T>::value) {
            if (cantidad_frontera < 4) {
                throw std::invalid_argument(
                    "La frontera entera estrictamente convexa requiere "
                    "al menos cuatro puntos"
                );
            }

            const long long ultimo_x =
                static_cast<long long>(cantidad_frontera - 1);
            std::uniform_int_distribution<long long> distribucion_x(
                1, ultimo_x - 1
            );
            for (std::size_t indice = 0; indice < cantidad_interior; ++indice) {
                const long long coordenada_x = distribucion_x(generador);
                const long long limite_inferior = coordenada_x * coordenada_x;
                const long long limite_superior = ultimo_x * coordenada_x;
                std::uniform_int_distribution<long long> distribucion_y(
                    limite_inferior + 1, limite_superior - 1
                );
                nube.emplace_back(
                    static_cast<T>(coordenada_x),
                    static_cast<T>(distribucion_y(generador))
                );
            }
            return;
        }

        if (forma_frontera == BoundaryShape::StrictlyConvex) {
            const double pi = std::acos(-1.0);
            std::uniform_real_distribution<double> distribucion_angulos(
                0.0, 2.0 * pi
            );
            std::uniform_real_distribution<double> distribucion_radios(0.0, 1.0);
            for (std::size_t indice = 0; indice < cantidad_interior; ++indice) {
                const double angulo = distribucion_angulos(generador);
                const double radio =
                    0.8 * std::sqrt(distribucion_radios(generador));
                nube.emplace_back(
                    static_cast<T>(radio * std::cos(angulo)),
                    static_cast<T>(radio * std::sin(angulo))
                );
            }
            return;
        }

        const double extension = std::is_integral<T>::value
            ? static_cast<double>(cantidad_frontera + 1)
            : 1.0;
        std::uniform_real_distribution<double> distribucion_coordenadas(
            -0.5 * extension, 0.5 * extension
        );
        for (std::size_t indice = 0; indice < cantidad_interior; ++indice) {
            nube.emplace_back(
                convertir_coordenada(distribucion_coordenadas(generador)),
                convertir_coordenada(distribucion_coordenadas(generador))
            );
        }
    }

public:
    BoundaryInteriorPointCloudGenerator(
        double porcentaje_frontera,
        BoundaryShape forma_frontera
    ) : porcentaje(porcentaje_frontera), forma_frontera(forma_frontera) {
        if (porcentaje <= 0.0 || porcentaje > 100.0) {
            throw std::invalid_argument(
                "El porcentaje de frontera debe estar en (0, 100]"
            );
        }
    }

    std::vector<Punto<T>> generar(
        std::size_t cantidad_puntos,
        std::mt19937& generador
    ) const override {
        // Redondea el porcentaje para obtener una cantidad entera de puntos.
        const std::size_t cantidad_frontera = static_cast<std::size_t>(
            std::llround(cantidad_puntos * porcentaje / 100.0)
        );
        const std::size_t cantidad_interior = cantidad_puntos - cantidad_frontera;
        std::vector<Punto<T>> nube;
        nube.reserve(cantidad_puntos);

        if (forma_frontera == BoundaryShape::CollinearSides) {
            agregar_frontera_lados_colineales(nube, cantidad_frontera);
        } else {
            agregar_frontera_estrictamente_convexa(
                nube, cantidad_frontera, generador
            );
        }
        agregar_puntos_interiores(
            nube, cantidad_frontera, cantidad_interior, generador
        );
        return nube;
    }

    std::string nombre() const override {
        const char* tipo_coordenada =
            std::is_integral<T>::value ? "entero" : "real";
        const char* nombre_forma = forma_frontera == BoundaryShape::CollinearSides
            ? "lados_colineales"
            : "frontera_estrictamente_convexa";
        return std::string(tipo_coordenada) + "_" + nombre_forma;
    }

    double porcentaje_frontera() const override {
        return porcentaje;
    }
};
