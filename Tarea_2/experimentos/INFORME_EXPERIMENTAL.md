# Informe experimental: algoritmos de cerradura convexa

## Problema y objetivo

Para una nube de puntos, escoger entre `IncrementalAlgorithm` (ordenar y construir la cerradura) y `RandomIncrementalAlgorithm` (insertar en orden aleatorio) puede cambiar el tiempo de cálculo según el tamaño y la cantidad de puntos que realmente queden en la frontera. El experimento busca caracterizar esa elección, no asumir que un método es siempre superior.

## Hipótesis en forma de pregunta

1. **Nube aleatoria:** ¿Para nubes uniformes sin un porcentaje de frontera impuesto, cuál de los dos algoritmos resuelve más rápido el problema de hallar la cerradura al aumentar `n` hasta `10^6`?
2. **Umbral de puntos extremos:** ¿Hasta qué porcentaje de puntos en una frontera estrictamente convexa conviene usar el método random incremental, antes de que el algoritmo incremental por ordenamiento sea más rápido?
3. **Colinealidad y tipo de coordenada:** ¿Cambian la elección o el costo relativo cuando los puntos de frontera son colineales en vez de estrictamente convexos, y cuando las coordenadas son enteras en vez de reales?

Estas preguntas resuelven la decisión práctica de qué estrategia usar para cada distribución de entrada y si existe un umbral porcentual general.

## Estrategias de generación de puntos

Los generadores implementan `PointCloudGenerator<T>` (patrón Strategy); ambos reciben `n` y un `std::mt19937`, y producen exactamente `n` puntos:

- `UniformPointCloudGenerator<T>` genera puntos uniformes. Para `double`, usa coordenadas continuas en `[-1,1]`; para `long long`, usa coordenadas enteras en `[-1 000 000,1 000 000]`.
- `BoundaryInteriorPointCloudGenerator<T>` recibe un porcentaje. Genera esa cantidad de puntos en la frontera y pone los restantes en el interior. Permite dos formas:
  - `StrictlyConvex`: círculo para reales y parábola entera estrictamente convexa para enteros; todos los puntos generados en la frontera son vértices extremos.
  - `CollinearSides`: distribuye puntos sobre los lados de un cuadrado; los puntos intermedios de cada lado pertenecen a la frontera pero no son vértices extremos.

El número de puntos en la frontera se redondea al entero más cercano. En la parábola entera, los puntos interiores se generan entre la curva y la cuerda superior. En el cuadrado, se generan dentro del cuadrado. Las estrategias se pueden extender implementando `PointCloudGenerator<T>`.

## Protocolo experimental

- Compilador: `g++ 15.2.0`, C++17, opciones `-O2 -Wall -Wextra`.
- Tamaños: `n = 10 000`, `100 000` y `1 000 000`.
- Cinco repeticiones por escenario, con semilla maestra `20261022`; las nubes se generan antes de cronometrar. Cada algoritmo recibe la misma nube y se usa una semilla aleatoria reproducible por ejecución del algoritmo aleatorizado.
- Se comparan coordenadas `double` y `long long`, y se verifica en cada repetición que ambos algoritmos devuelvan el mismo conjunto de vértices. Si difieren, el experimento falla.
- En `n = 10 000` se probaron fronteras estrictamente convexas de `0.1 %`, `0.15 %`, `0.2 %`, `0.3 %`, `0.5 %`, `1 %`, `10 %` y `70 %`; además, fronteras con lados colineales de `10 %` y `70 %`.
- En `n = 100 000`, se probaron fronteras estrictamente convexas de `0.1 %` y `1 %`; en `n = 1 000 000`, se probó `0.01 %`. El número de pruebas de porcentaje se reduce para tamaños mayores porque el costo del algoritmo random incremental crece con el número de vértices de la frontera.

Los tiempos reportados son promedios en milisegundos. Los resultados completos por escenario están en [resultados_experimento.csv](./resultados_experimento.csv).

## Gráfico comparativo

![Comparación gráfica de tiempos por porcentaje de frontera, tamaño y colinealidad](./grafico_resultados.svg)

En el panel A se usa escala logarítmica para que se distingan tanto los tiempos bajos como el aumento pronunciado del random incremental cuando casi todos los puntos son extremos. El panel B compara el crecimiento en nubes uniformes; el panel C muestra por qué un alto porcentaje de puntos sobre el borde no implica necesariamente muchos vértices cuando hay colinealidad.

## Resultados: nubes uniformes

| Coordenadas | n | Vértices de la cerradura | Incremental (ms) | Random incremental (ms) | Más rápido en esta corrida |
|---|---:|---:|---:|---:|---|
| Reales | 10 000 | 21 | 0.639 | 0.581 | Random |
| Enteras | 10 000 | 26 | 0.523 | 0.647 | Incremental |
| Reales | 100 000 | 31 | 7.692 | 6.471 | Random |
| Enteras | 100 000 | 29 | 6.027 | 6.336 | Incremental |
| Reales | 1 000 000 | 30 | 89.274 | 78.403 | Random |
| Enteras | 1 000 000 | 37 | 72.025 | 78.076 | Incremental |

En nubes uniformes reales, el random incremental fue entre aproximadamente 9 % y 19 % más rápido en estas mediciones; en las enteras, el incremental fue entre aproximadamente 5 % y 24 % más rápido. Son diferencias pequeñas y no prueban una ventaja general: dependen de la muestra, el equipo y la precisión/tipo numérico. Las fronteras observadas fueron pequeñas comparadas con `n`.

## Resultados: frontera estrictamente convexa

### n = 10 000

| Porcentaje | Vértices | Incremental real (ms) | Random real (ms) | Incremental entero (ms) | Random entero (ms) |
|---:|---:|---:|---:|---:|---:|
| 0.1 % | 10 | 0.616 | 0.538 | 0.408 | 0.443 |
| 0.15 % | 15 | 0.661 | 0.511 | 0.414 | 0.395 |
| 0.2 % | 20 | 0.618 | 0.537 | 0.500 | 0.468 |
| 0.3 % | 30 | 0.710 | 0.501 | 0.505 | 0.607 |
| 0.5 % | 50 | 0.682 | 0.651 | 0.531 | 0.724 |
| 1 % | 100 | 0.618 | 1.086 | 0.648 | 1.441 |
| 10 % | 1 000 | 0.649 | 9.183 | 0.577 | 9.715 |
| 70 % | 7 000 | 0.593 | 116.004 | 0.549 | 110.098 |

### Tamaños mayores

| n | Porcentaje | Vértices | Incremental real (ms) | Random real (ms) | Incremental entero (ms) | Random entero (ms) |
|---:|---:|---:|---:|---:|---:|---:|
| 100 000 | 0.1 % | 100 | 7.875 | 10.087 | 6.281 | 12.459 |
| 100 000 | 1 % | 1 000 | 7.958 | 79.441 | 6.175 | 80.041 |
| 1 000 000 | 0.01 % | 100 | 89.380 | 104.797 | 64.929 | 150.506 |

### Frontera con lados colineales, n = 10 000

| Coordenadas | Porcentaje de puntos en la frontera | Vértices extremos | Incremental (ms) | Random incremental (ms) |
|---|---:|---:|---:|---:|
| Reales | 10 % | 4 | 0.687 | 0.298 |
| Reales | 70 % | 4 | 0.408 | 0.300 |
| Enteras | 10 % | 4 | 0.559 | 0.278 |
| Enteras | 70 % | 4 | 0.339 | 0.289 |

Los puntos intermedios colineales de cada lado pertenecen al borde geométrico, pero los algoritmos devuelven solo los vértices extremos; por eso la cerradura de estos casos tiene cuatro vértices aunque aumente el porcentaje de muestras en la frontera.

## Análisis de las hipótesis

1. **Nube aleatoria:** no hay ganador independiente del tipo. El random incremental fue más rápido para las tres nubes reales medidas y el incremental para las tres enteras. Las diferencias fueron moderadas; no se justifica escoger el algoritmo solo por saber que la nube es “aleatoria”.
2. **Umbral de puntos extremos:** aparece un límite, pero no un porcentaje universal. Para `n = 10 000`, el random incremental aún fue competitivo cerca de `0.5 %` en reales, pero a `1 %` ya fue más lento. En enteros, las pequeñas fluctuaciones hacen que el cruce observado esté entre `0.2 %` y `0.3 %`. Al crecer `n`, el random incremental fue más lento incluso con `0.1 %` en `n = 100 000` y con `0.01 %` en `n = 1 000 000`. El factor determinante es el número absoluto de vértices de la cerradura junto con `n`, no solo el porcentaje.
3. **Colinealidad y tipo numérico:** con 10 % o 70 % de puntos sobre lados colineales, la frontera solo tenía cuatro vértices y el random incremental fue más rápido en esta corrida. Con frontera estrictamente convexa, el número de vértices creció con el porcentaje y el random incremental se degradó rápidamente. Ambos métodos coincidieron en todas las pruebas reportadas, incluyendo los casos con puntos colineales y sin puntos colineales. No obstante, el resultado del algoritmo representa vértices extremos, no todos los puntos que pertenecen al borde; no se debe esperar que preserve los puntos intermedios de un segmento colineal.

## Conclusión y limitaciones

En distribuciones uniformes que producen una cerradura pequeña, ambos métodos tienen tiempos comparables y las mediciones no dan un ganador robusto. Si se espera que una parte apreciable de la entrada forme una frontera estrictamente convexa, conviene el algoritmo incremental por ordenamiento: su tiempo se mantuvo cercano a `O(n log n)`, mientras que el random incremental revisa la frontera en cada inserción y puede acercarse a `O(nh)` (`h` es el número de vértices), con degradación marcada desde porcentajes pequeños al aumentar `n`.

El umbral medido cambia con `n`, tipo y geometría, por lo que no existe un porcentaje único deducible de estas corridas. Además, la comparación entero/real no aísla solo el tipo: la nube entera uniforme es discreta y la frontera estrictamente convexa se construye con una parábola, mientras la real usa coordenadas continuas y un círculo. Los resultados son evidencia experimental para este compilador y configuración, no una garantía estadística universal; conviene repetirlos en el equipo de evaluación.
