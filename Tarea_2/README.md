# Tarea 2: Cerradura convexa

Implementación y comparación de dos algoritmos para calcular la cerradura convexa.

## Estructura

- [`tarea1/`](./tarea1/): clases reutilizadas de la Tarea 1 (`Punto`, `Vector` y `Poligono`) y su enunciado.
- [`algorithms/`](./algorithms/): interfaz `ConvexHullStrategy` y algoritmos incremental y random incremental.
- [`experiments/`](./experiments/): generadores Strategy, pruebas, ejecutable de benchmarks, informe, gráfico y resultados CSV.
- [`tarea2_CH.pdf`](./tarea2_CH.pdf): enunciado de la Tarea 2.

## Pruebas

Requiere un compilador compatible con C++17, como `g++`. Ejecuta los comandos desde esta carpeta:

### Windows (PowerShell)

```powershell
g++ -std=c++17 -Wall -Wextra experiments\test.cpp -o test.exe
.\test.exe
```

### Linux/macOS

```bash
g++ -std=c++17 -Wall -Wextra experiments/test.cpp -o test
./test
```

La suite cubre las clases geométricas, casos de cerradura para `int`, `float` y `double`, 500 permutaciones con semillas reproducibles y los generadores de puntos enteros/reales con fronteras colineales y estrictamente convexas.

## Experimentos

El ejecutable compara ambos algoritmos en nubes reales y enteras, para tamaños `n = 10^4, 10^5, 10^6`, diversos porcentajes de puntos en la frontera y dos formas de frontera. Cada repetición comprueba que ambos métodos devuelvan el mismo conjunto de vértices.

### Windows (PowerShell)

```powershell
g++ -std=c++17 -O2 -Wall -Wextra experiments\main_experimento.cpp -o experimento.exe
.\experimento.exe
```

### Linux/macOS

```bash
g++ -std=c++17 -O2 -Wall -Wextra experiments/main_experimento.cpp -o experimento
./experimento
```

Por defecto ejecuta cinco repeticiones con semilla `20261022` y guarda los resultados en `experiments/resultados_experimento.csv`. Se pueden indicar exponente máximo, repeticiones, archivo CSV y semilla:

```text
experimento [exponente_maximo] [repeticiones] [archivo.csv] [semilla]
```

Por ejemplo, `experimento 5 3 experiments/prueba.csv 1234` ejecuta los tamaños hasta `10^5`.

El análisis de hipótesis, el protocolo y las conclusiones están en [`experiments/INFORME_EXPERIMENTAL.md`](./experiments/INFORME_EXPERIMENTAL.md), junto con el [gráfico](./experiments/grafico_resultados.svg) y los [datos completos](./experiments/resultados_experimento.csv).
