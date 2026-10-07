#pragma once

#include "convexhullstrategy.hpp"
#include "vector.hpp"
#include <algorithm>
#include <vector>

template <typename T>
class IncrementalAlgorithm : public ConvexHullStrategy<T> {
private:
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

public:
    Poligono<T> apply(const std::vector<Punto<T>>& nube_puntos) const override {
        std::vector<Punto<T>> points = nube_puntos;
        std::vector<Punto<T>> puntos_inferiores;
        std::vector<Punto<T>> puntos_superiores;
        

        // Oredenasmos la nube de puntos de izquierda a derecha y de abajo hacia arriba.
        std::sort(points.begin(), points.end(), [](const Punto<T>& a, const Punto<T>& b) {
                if (a.get_x() != b.get_x())
                    return a.get_x() < b.get_x();
                return a.get_y() < b.get_y();
            });

        // Quita puntos repetidos o solapados entre ellos en el plano XY.
        points.erase(
            std::unique(points.begin(), points.end(),
                [](const Punto<T>& a, const Punto<T>& b) {
                    return a.get_x() == b.get_x()
                        && a.get_y() == b.get_y();
                }),
            points.end()
        );
        // Si hay 2 o menos puntos restantes devolvemos la linea o punto restante como un "poligono".
        if (points.size() <= 2)
            return Poligono<T>(points);

        
        // Recorre los puntos ordenados de izquierda a derecha para construir la cadena inferior de la cerradura convexa.
        for (const Punto<T>& p : points) {
            // Mientras los dos últimos vértices y p no formen un giro antihorario, el último vértice no pertenece al borde inferior y se elimina de los puntos inferiores.
            while (puntos_inferiores.size() >= 2 && cross(puntos_inferiores[puntos_inferiores.size() - 2], puntos_inferiores.back(), p) <= 0) {
                puntos_inferiores.pop_back();
            }
            // Agrega p después de eliminar los vértices que quedaban hacia adentro.
            puntos_inferiores.push_back(p);
        }

        

        // Recorre los puntos en orden inverso, de derecha a izquierda, para construir la cadena superior de la cerradura convexa.
        for (auto it = points.rbegin(); it != points.rend(); ++it) {
            // Si los dos últimos vértices y el punto actual no forman un giro antihorario, elimina el último vértice.
            while (puntos_superiores.size() >= 2 &&
                   cross(puntos_superiores[puntos_superiores.size() - 2], puntos_superiores.back(), *it) <= 0) {
                puntos_superiores.pop_back();
            }
            // Agrega el punto actual a la cadena superior.
            puntos_superiores.push_back(*it);
        }

        // Los extremos aparecen en ambas listas; se quitan antes de unirlas.
        puntos_inferiores.pop_back();
        puntos_superiores.pop_back();
        puntos_inferiores.insert(puntos_inferiores.end(), puntos_superiores.begin(), puntos_superiores.end());

        return Poligono<T>(puntos_inferiores);
    }
};
