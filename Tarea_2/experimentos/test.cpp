#include "../tarea1/punto.cpp"
#include "../tarea1/vector.cpp"
#include "../tarea1/poligono.cpp"
#include "../algorithms/incrementalalgorithm.hpp"
#include "../algorithms/randomincrementalalgorithm.hpp"
#include "pointcloudgenerator.hpp"
#include <algorithm>
#include <cassert>
#include <ostream>
#include <cmath>
#include <random>
#include <string>
#include <vector>

double epsilon = 1e-9;

bool casi_igual(double valor, double esperado) {
    double tolerancia = epsilon * std::max(1.0, std::abs(esperado));
    return std::abs(valor - esperado) <= tolerancia;
}

void test_punto(){
    Punto<double> p1(0.0, 0.0);
    Punto<double> p2(3.0, 4.0);
    Punto<double> p3(p2);

    std::cout << "Test de igualdad\n";
    assert(p2 == p3);
    std::cout << "[OK] Igualdad\n";

    std::cout << "Test de desigualdad\n";
    assert(!(p1 == p3));
    std::cout << "[OK] Desigualdad\n";

    std::cout << "Test de distancia entre puntos\n";
    
    assert(casi_igual(p1.distancia(p2), 5.0));

    assert(casi_igual(p2.distancia(p3),0));
    std::cout <<"[OK] Distancia\n";

    Punto<double> p4(-1e150, 2e150, -3e150);
    Punto<double> p5(-1e150 + 3e150, 2e150 - 4e150, -3e150);
    assert(casi_igual(p4.distancia(p5), 5e150));

    Punto<double> p6(1e-150, -2e-150, 3e-150);
    Punto<double> p7(4e-150, 2e-150, 3e-150);
    assert(casi_igual(p6.distancia(p7), 5e-150));
    assert(Punto<double>(-4.0, -5.0, -6.0) == Punto<double>(-4.0, -5.0, -6.0));
    std::cout << "[OK] Punto: negativos y escalas extremas\n";

}

void test_vector(){
    Punto<double> p1(0.0, 0.0);
    Vector<double> v1(1.0, 2.0);
    Vector<double> v2(3.0, 4.0);
    Vector<double> v3(v2);
    Vector<double> v4(p1);
    Vector<double> v5 = v1.producto_cruz(v2);
    Vector<double> v6 = v1 + v2;
    Vector<double> v7 = v1 * 2.0;
    Vector<double> v8(-1e150, 2e150, -3e150);
    Vector<double> v9(4e150, -5e150, 6e150);
    Vector<double> v10(1e-150, -2e-150, 3e-150);
    Vector<double> v11(-4e-150, 5e-150, -6e-150);
    Vector<double> cruz = Vector<double>(1.0, 2.0, 3.0).producto_cruz(
        Vector<double>(4.0, 5.0, 6.0));

    std::cout << "Test de igualdad\n";
    assert(v2 == v3);
    assert(v4.get_origen() == p1);
    std::cout << "[OK] Igualdad\n";

    std::cout << "Test de desigualdad\n";
    assert(!(v1 == v3));
    std::cout << "[OK] Desigualdad\n";

    std::cout << "Test de magnitud de vector\n";
    
    assert(std::abs(v1.magnitud() - sqrt(5.0))==0);
    std::cout <<"Magnitud obtenida: "<< v1.magnitud()<< "\n";

    std::cout << "Test de magnitud de vector\n";
    assert(std::abs(v2.magnitud() - 5.0) < epsilon);
    assert(casi_igual(v8.magnitud(), std::sqrt(14e300)));
    std::cout <<"[OK] Magnitud\n";

    std::cout << "Test de producto punto\n";
    assert(casi_igual(v1.producto_punto(v2), 11.0));
    assert(casi_igual(v8.producto_punto(v9), -32e300));
    assert(casi_igual(v10.producto_punto(v11), -32e-300));
    std::cout <<"[OK] Producto punto\n";
    
    std::cout << "Test de producto cruz\n";
  
    assert(v5 == Vector<double>(0.0, 0.0, -2.0));
    assert(cruz == Vector<double>(-3.0, 6.0, -3.0));
    std::cout << "[OK] Producto cruz\n";



    std::cout << "Test de suma de vectores\n";
    assert(v6 == Vector<double>(4.0, 6.0));
    std::cout <<"[OK] Suma\n";



    
    std::cout << "Test de multiplicacion por escalar \n";
    assert(v7 == Vector<double>(2.0, 4.0));
    assert((Vector<double>(-1.0, 2.0, -3.0) * -2.0) ==
           Vector<double>(2.0, -4.0, 6.0));
    std::cout <<"[OK] Multiplicacion\n";

}


void test_poligono(){

    // pentagono CCW
    std::vector<Punto<double>> pts = {
        Punto<double>(0.0, 0.0),
        Punto<double>(1.0, 0.0),
        Punto<double>(1.0, 1.0),
        Punto<double>(0.5, 1.5),
        Punto<double>(0.0, 1.0)
    };
    Poligono<double> pol(pts);

    assert(pol.size() == 5);
    assert(std::abs(pol.area() - 1.25) < 1e-9);
    assert(pol.es_ccw());

    // Test toCCW: invertir y verificar que se corrige
    std::vector<Punto<double>> ptsCW = {
        Punto<double>(0.0, 1.0),
        Punto<double>(0.5, 0.2),
        Punto<double>(1.0, 1.0),
        Punto<double>(1.0, 0.0),
        Punto<double>(0.0, 0.0)
    };
    Poligono<double> polCW(ptsCW);
    std::cout << "Poligono CW: " << polCW << " es CCW? " << polCW.es_ccw() << "\n";
    assert(!polCW.es_ccw());
    polCW.invertir();
    std::cout << "Poligono invertido: " << polCW << " es CCW? " << polCW.es_ccw() << "\n";
    assert(polCW.es_ccw());

    std::cout << "Poligono: " << pol << "\n";
    std::cout << "[OK] Poligono\n";

    std::vector<Punto<double>> ptsNegativos = {
        Punto<double>(-3.0, -2.0),
        Punto<double>(-1.0, -2.0),
        Punto<double>(-1.0, 1.0),
        Punto<double>(-3.0, 1.0)
    };
    Poligono<double> polNegativos(ptsNegativos);
    assert(polNegativos.es_ccw());
    assert(casi_igual(polNegativos.area(), 6.0));

    std::vector<Punto<double>> ptsGrande = {
        Punto<double>(-2e100, -1e100),
        Punto<double>(2e100, -1e100),
        Punto<double>(2e100, 1e100),
        Punto<double>(-2e100, 1e100)
    };
    Poligono<double> polGrande(ptsGrande);
    assert(casi_igual(polGrande.area(), 8e200));
    std::cout << "[OK] Poligono: negativos y escala grande\n";
}

template <typename T>
bool tiene_mismos_vertices(
    const Poligono<T>& hull,
    const std::vector<Punto<T>>& expected
) {
    if (hull.size() != static_cast<int>(expected.size())) {
        return false;
    }

    for (int i = 0; i < hull.size(); ++i) {
        bool found = false;
        for (const Punto<T>& point : expected) {
            if (hull[i].get_x() == point.get_x() &&
                hull[i].get_y() == point.get_y()) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

template <typename T>
bool tiene_mismos_vertices(
    const Poligono<T>& first,
    const Poligono<T>& second
) {
    if (first.size() != second.size()) {
        return false;
    }

    for (int i = 0; i < first.size(); ++i) {
        bool found = false;
        for (int j = 0; j < second.size(); ++j) {
            if (first[i].get_x() == second[j].get_x() &&
                first[i].get_y() == second[j].get_y()) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

template <typename T>
void test_convex_hull_case(
    const std::string& name,
    const std::vector<Punto<T>>& cloud,
    const std::vector<Punto<T>>& expected
) {
    IncrementalAlgorithm<T> incremental;
    const Poligono<T> expected_hull = incremental.apply(cloud);
    assert(tiene_mismos_vertices(expected_hull, expected));

    // El algoritmo aleatorio puede producir un orden de inserción distinto;
    // repetir la prueba verifica que cada permutación conserva la misma hull.
    for (int attempt = 0; attempt < 5; ++attempt) {
        RandomIncrementalAlgorithm<T> random_incremental(
            static_cast<unsigned int>(20261022 + attempt)
        );
        const Poligono<T> actual_hull = random_incremental.apply(cloud);
        assert(tiene_mismos_vertices(actual_hull, expected));
    }

    std::cout << "[OK] Cerradura convexa: " << name << '\n';
}

template <typename T>
void test_convex_hull_algorithms_for_type(const std::string& type_name) {
    test_convex_hull_case<T>(type_name + ": nube vacia", {}, {});

    test_convex_hull_case<T>(
        type_name + ": un punto",
        {{T(2), T(3)}},
        {{T(2), T(3)}}
    );

    test_convex_hull_case<T>(
        type_name + ": puntos repetidos iguales",
        {{T(2), T(3)}, {T(2), T(3)}, {T(2), T(3)}},
        {{T(2), T(3)}}
    );

    test_convex_hull_case<T>(
        type_name + ": dos puntos distintos",
        {{T(4), T(1)}, {T(-1), T(2)}},
        {{T(-1), T(2)}, {T(4), T(1)}}
    );

    test_convex_hull_case<T>(
        type_name + ": nube colineal",
        {{T(1), T(1)}, {T(-2), T(-2)}, {T(3), T(3)},
         {T(0), T(0)}, {T(1), T(1)}},
        {{T(-2), T(-2)}, {T(3), T(3)}}
    );

    test_convex_hull_case<T>(
        type_name + ": triangulo con puntos interiores",
        {{T(0), T(0)}, {T(4), T(0)}, {T(0), T(3)},
         {T(1), T(1)}, {T(2), T(0)}, {T(0), T(0)}},
        {{T(0), T(0)}, {T(4), T(0)}, {T(0), T(3)}}
    );

    test_convex_hull_case<T>(
        type_name + ": cuadrado con interiores, bordes y duplicados",
        {{T(0), T(0)}, {T(2), T(0)}, {T(2), T(2)}, {T(0), T(2)},
         {T(1), T(1)}, {T(1), T(0)}, {T(2), T(1)}, {T(0), T(0)}},
        {{T(0), T(0)}, {T(2), T(0)}, {T(2), T(2)}, {T(0), T(2)}}
    );

    test_convex_hull_case<T>(
        type_name + ": poligono convexo irregular",
        {{T(0), T(0)}, {T(4), T(0)}, {T(5), T(2)}, {T(3), T(5)},
         {T(0), T(4)}, {T(2), T(2)}, {T(1), T(1)}, {T(3), T(1)}},
        {{T(0), T(0)}, {T(4), T(0)}, {T(5), T(2)}, {T(3), T(5)},
         {T(0), T(4)}}
    );
}

void test_convex_hull_algorithms() {
    test_convex_hull_algorithms_for_type<int>("int");
    test_convex_hull_algorithms_for_type<float>("float");
    test_convex_hull_algorithms_for_type<double>("double");
    std::cout << "[OK] Todos los tests de cerradura convexa pasaron.\n\n";
}

void test_randomized_convex_hulls() {
    std::mt19937 generator(20261022U);
    std::uniform_int_distribution<int> coordinate(-100, 100);
    IncrementalAlgorithm<int> incremental;
    RandomIncrementalAlgorithm<int> random_default_seed;
    const std::vector<Punto<int>> default_seed_cloud = {
        {0, 0}, {4, 0}, {4, 3}, {0, 3}, {2, 1}
    };
    Poligono<int> default_seed_hull = random_default_seed.apply(default_seed_cloud);
    assert(tiene_mismos_vertices(
        default_seed_hull,
        std::vector<Punto<int>>{{0, 0}, {4, 0}, {4, 3}, {0, 3}}
    ));

    for (int case_index = 0; case_index < 100; ++case_index) {
        std::vector<Punto<int>> cloud;
        for (int point_index = 0; point_index < 30; ++point_index) {
            cloud.emplace_back(coordinate(generator), coordinate(generator));
        }
        if (case_index % 2 == 0) {
            cloud.push_back(cloud.front());
        }

        const Poligono<int> expected_hull = incremental.apply(cloud);
        for (unsigned int seed = 0; seed < 5; ++seed) {
            RandomIncrementalAlgorithm<int> random_incremental(
                8675309U + static_cast<unsigned int>(case_index) * 5U + seed
            );
            Poligono<int> actual_hull = random_incremental.apply(cloud);
            assert(tiene_mismos_vertices(actual_hull, expected_hull));
            assert(actual_hull.size() < 3 || actual_hull.es_ccw());
        }
    }

    std::cout << "[OK] 500 ejecuciones con semillas distintas contra el algoritmo incremental.\n";
}

template <typename T>
void test_cloud_generator_strategies() {
    std::mt19937 generator(141422U);
    const UniformPointCloudGenerator<T> uniform;
    const std::vector<Punto<T>> uniform_cloud = uniform.generate(100, generator);
    assert(uniform_cloud.size() == 100);

    const BoundaryInteriorPointCloudGenerator<T> strict(
        10.0, BoundaryShape::StrictlyConvex
    );
    const std::vector<Punto<T>> strict_cloud = strict.generate(100, generator);
    assert(strict_cloud.size() == 100);
    const Poligono<T> strict_hull = IncrementalAlgorithm<T>().apply(strict_cloud);
    assert(strict_hull.size() == 10);

    const BoundaryInteriorPointCloudGenerator<T> collinear(
        10.0, BoundaryShape::CollinearSides
    );
    const std::vector<Punto<T>> collinear_cloud =
        collinear.generate(100, generator);
    assert(collinear_cloud.size() == 100);
    const Poligono<T> collinear_hull =
        IncrementalAlgorithm<T>().apply(collinear_cloud);
    assert(collinear_hull.size() < 10);
    assert(collinear_hull.size() >= 4);
}

void test_cloud_generator_strategies() {
    test_cloud_generator_strategies<double>();
    test_cloud_generator_strategies<long long>();
    std::cout << "[OK] Estrategias de generación real/entera y fronteras colineales/convexas.\n";
}

int main() {
    
    test_punto();
    std::cout << "[OK] Todos los tests de PUNTO pasaron exitosamente.\n\n";
    test_vector();
    std::cout << "[OK] Todos los tests de VECTOR pasaron exitosamente.\n\n";
    test_poligono();
    std::cout << "[OK] Todos los tests de POLIGONO pasaron exitosamente.\n\n";
    test_convex_hull_algorithms();
    test_randomized_convex_hulls();
    test_cloud_generator_strategies();
    std::cout << "Todos los tests pasaron exitosamente.\n";
    return 0;
}