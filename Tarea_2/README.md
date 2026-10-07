# Tarea 1: Introducción a C++

Implementación de clases básicas de geometría computacional: puntos, vectores y polígonos.

## Estructura

- `punto.hpp` y `punto.cpp`: clase genérica para representar puntos.
- `vector.hpp` y `vector.cpp`: clase genérica para representar vectores y realizar operaciones vectoriales.
- `poligono.hpp` y `poligono.cpp`: clase genérica para representar polígonos, calcular su área y orientación.
- `test.cpp`: pruebas de las tres clases.

## Tests

Los tests de las clases se encuentran en el archivo `test.cpp`. Incluye pruebas para `Punto`, `Vector` y `Poligono`.

Se requiere un compilador compatible con C++17, como `g++`.

## Ejecución

Desde la carpeta del proyecto, compile el archivo de tests con:

### Windows

```powershell
g++ -std=c++17 -Wall -Wextra test.cpp -o test.exe
.\test.exe
```

### Linux/macOS

```bash
g++ -std=c++17 -Wall -Wextra test.cpp -o test
./test
```
