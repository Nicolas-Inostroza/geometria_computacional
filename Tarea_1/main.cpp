#include "punto.cpp"
#include "vector.cpp"
#include "poligono.cpp"
#include <cassert>
#include <ostream>
#include <cmath>

double epsilon = 1e-9;

bool casi_igual(double valor, double esperado) {
    double tolerancia = epsilon * std::max(1.0, std::abs(esperado));
    return std::abs(valor - esperado) <= tolerancia;
}

void test_punto(){
    Punto<double> p1(0.0, 0.0);
    Punto<double> p2(3.0, 4.0);
    Punto<double> p3(p2);

    std::cout << "Test de igualdad" <<p2 << " == " << p3 << "\n";
    assert(p2 == p3);
    std::cout << "[OK] Igualdad\n";

    std::cout << "Test de desigualdad" <<p1 << " != " << p3 << "\n";
    assert(!(p1 == p3));
    std::cout << "[OK] Desigualdad\n";

    std::cout << "Test de distancia entre puntos" <<p1 << " y " << p2 << "\n";
    
    assert(std::abs(p1.distancia(p2) - 5.0) < epsilon);
    std::cout <<"Distancioa obtenida: "<< p1.distancia(p2)<< "\n";

    std::cout << "Test de distancia entre puntos" <<p2 << " y " << p3 << "\n";
    assert(std::abs(p2.distancia(p3))==0);
    std::cout <<"Distancioa obtenida: "<< p2.distancia(p3)<< "\n" << "[OK] Distancia\n";
    std::cout << "[OK] Punto\n";

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

    std::cout << "Test de igualdad" <<v2 << " == " << v3 << "\n";
    assert(v2 == v3);
    assert(v4.get_origen() == p1);
    std::cout << "[OK] Igualdad\n";

    std::cout << "Test de desigualdad" <<v1 << " != " << v3 << "\n";
    assert(!(v1 == v3));
    std::cout << "[OK] Desigualdad\n";

    std::cout << "Test de magnitud de vector" <<v1 << "\n";
    
    assert(std::abs(v1.magnitud() - sqrt(5.0))==0);
    std::cout <<"Magnitud obtenida: "<< v1.magnitud()<< "\n";

    std::cout << "Test de magnitud de vector" <<v2 << "\n";
    assert(std::abs(v2.magnitud() - 5.0) < epsilon);
    std::cout <<"Magnitud obtenida: "<< v2.magnitud()<< "\n" << "[OK] Magnitud\n";

    std::cout << "Test de producto punto" <<v1 << " . " << v2 << "\n";
    assert(std::abs(v1.producto_punto(v2) - 11.0) < epsilon);
    std::cout <<"Producto punto obtenido: "<< v1.producto_punto(v2)<< "\n" << "[OK] Producto punto\n";
    
    std::cout << "Test de producto cruz" <<v1 << " x " << v2 << "\n";
    Vector<double> v5 = v1.producto_cruz(v2);
    std::cout <<"Producto cruz obtenido: "<< v5 << "\n" << "[OK] Producto cruz\n";
    std::cout << "Test de suma de vectores" <<v1 << " + " << v2 << "\n";
    Vector<double> v6 = v1 + v2;
    assert(v6 == Vector<double>(4.0, 6.0));
    std::cout <<"Suma obtenida: "<< v6 << "\n" << "[OK] Suma\n";
    std::cout << "Test de multiplicacion por escalar " <<v1 << " * 2.0\n";
    Vector<double> v7 = v1 * 2.0;
    assert(v7 == Vector<double>(2.0, 4.0));
    std::cout <<"Multiplicacion obtenida: "<< v7 << "\n" << "[OK] Multiplicacion\n";
    std::cout << "[OK] Vector\n";

    Vector<double> v8(-1e150, 2e150, -3e150);
    Vector<double> v9(4e150, -5e150, 6e150);
    assert(casi_igual(v8.producto_punto(v9), -32e300));
    assert(casi_igual(v8.magnitud(), std::sqrt(14e300)));

    Vector<double> v10(1e-150, -2e-150, 3e-150);
    Vector<double> v11(-4e-150, 5e-150, -6e-150);
    assert(casi_igual(v10.producto_punto(v11), -32e-300));

    Vector<double> cruz = Vector<double>(1.0, 2.0, 3.0).producto_cruz(
        Vector<double>(4.0, 5.0, 6.0));
    assert(cruz == Vector<double>(-3.0, 6.0, -3.0));
    assert((Vector<double>(-1.0, 2.0, -3.0) * -2.0) ==
           Vector<double>(2.0, -4.0, 6.0));
    std::cout << "[OK] Vector: negativos, 3D y escalas extremas\n";

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

int main() {
    
    test_punto();
    std::cout << "[OK] Todos los tests pasaron exitosamente.\n\n";
    test_vector();
    std::cout << "[OK] Todos los tests pasaron exitosamente.\n\n";
    test_poligono();
    return 0;
}