# Tarea 2: Cerradura convexa

Implementación de dos algoritmos para calcular la cerradura convexa a partir de las clases de puntos, vectores y polígonos de la Tarea 1.

## Estructura

- `punto.hpp` y `punto.cpp`: clase genérica para representar puntos.
- `vector.hpp` y `vector.cpp`: clase genérica para representar vectores y realizar operaciones vectoriales.
- `poligono.hpp` y `poligono.cpp`: clase genérica para representar polígonos, calcular su área y orientación.
- `incrementalalgorithm.hpp` y `randomincrementalalgorithm.hpp`: estrategias de cerradura convexa.
- `randomincremental.hpp`: encabezado de compatibilidad para el nombre anterior.
- `pointcloudgenerator.hpp`: interfaz Strategy y generadores uniformes y con porcentaje configurable de puntos en la frontera.
- `test.cpp`: pruebas de las clases de la Tarea 1 y de ambos algoritmos.
- `main_experimento.cpp`: experimentos de tiempo y comparación de las cerraduras.

## Tests

Los tests se encuentran en `test.cpp`. Cubren `Punto`, `Vector`, `Poligono` y los dos algoritmos de cerradura convexa para diferentes tipos de coordenadas y casos geométricos. También comparan ambos algoritmos con 100 nubes generadas de forma reproducible y prueban cinco semillas por nube.

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

El programa compara `double` y `long long` para tamaños `n = 10^4, 10^5, 10^6`:

- nube aleatoria uniforme;
- nube con frontera estrictamente convexa en porcentajes desde 0.1 % hasta 70 % para `n = 10^4`, y porcentajes menores para tamaños mayores;
- frontera con lados colineales para `n = 10^4`.

En cada repetición mide ambos algoritmos sobre la misma nube, comprueba que devuelvan los mismos vértices y escribe los promedios en `resultados_experimento.csv`. Los generadores siguen `PointCloudGenerator<T>`, por lo que se pueden agregar nuevas distribuciones como estrategias.

La implementación random incremental recibe una semilla opcional; el experimento deriva una semilla reproducible para cada repetición. Las hipótesis, el protocolo y los resultados de referencia están en [INFORME_EXPERIMENTAL.md](./INFORME_EXPERIMENTAL.md).

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

Por defecto se prueban los exponentes 4 a 6, con 5 repeticiones. Se pueden indicar exponente máximo, repeticiones, archivo CSV y semilla:

```text
experimento [exponente_maximo] [repeticiones] [archivo.csv] [semilla]
```

Por ejemplo, `experimento 6 5 resultados.csv 1234` ejecuta el experimento hasta `n = 10^6`. El algoritmo random incremental recorre la frontera en cada inserción, así que puede tardar considerablemente cuando muchos puntos son vértices extremos.
