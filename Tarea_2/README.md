# Tarea 2: Cerradura convexa

Implementación de dos algoritmos para calcular la cerradura convexa a partir de las clases de puntos, vectores y polígonos de la Tarea 1.

## Estructura

- `punto.hpp` y `punto.cpp`: clase genérica para representar puntos.
- `vector.hpp` y `vector.cpp`: clase genérica para representar vectores y realizar operaciones vectoriales.
- `poligono.hpp` y `poligono.cpp`: clase genérica para representar polígonos, calcular su área y orientación.
- `incrementalalgorithm.hpp` y `randomincremental.hpp`: estrategias de cerradura convexa.
- `test.cpp`: pruebas de las clases de la Tarea 1 y de ambos algoritmos.
- `main_experimento.cpp`: experimentos de tiempo y comparación de las cerraduras.

## Tests

Los tests se encuentran en `test.cpp`. Cubren `Punto`, `Vector`, `Poligono` y los dos algoritmos de cerradura convexa para diferentes tipos de coordenadas y casos geométricos.

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

## Experimentos

El programa ejecuta dos escenarios para cada tamaño `n = 10^2, ..., 10^k`:

- puntos distribuidos uniformemente en un cuadrado;
- una nube en la que el 70 % de los puntos está sobre una circunferencia y el resto dentro de ella.

En cada repetición mide ambos algoritmos sobre la misma nube, comprueba que devuelvan los mismos vértices y escribe los promedios en `resultados_experimento.csv`.

### Windows

```powershell
g++ -std=c++17 -O2 -Wall -Wextra main_experimento.cpp -o experimento.exe
.\experimento.exe
```

### Linux/macOS

```bash
g++ -std=c++17 -O2 -Wall -Wextra main_experimento.cpp -o experimento
./experimento
```

Por defecto se prueban potencias hasta `10^4`, con 3 repeticiones. Se pueden indicar exponente máximo, repeticiones, archivo CSV y semilla:

```text
experimento [exponente_maximo] [repeticiones] [archivo.csv] [semilla]
```

Por ejemplo, `experimento 6 3 resultados.csv 1234` incluye los tamaños `10^2` a `10^6`. El algoritmo random incremental recorre la frontera en cada inserción, así que los casos con muchos puntos en la cerradura pueden tardar considerablemente. Para los resultados de mayor tamaño, ejecutar una sola repetición puede reducir el tiempo de medición.
