# 2019-1-grupo19
[![Build Status](https://travis-ci.com/pds2-dcc-ufmg/2019-1-grupo19.svg?token=Mnxg5DUtyLmLApyc8NAT&branch=master)](https://travis-ci.com/pds2-dcc-ufmg/2019-1-grupo19)

## Introdução

Esse projeto é um software open source para visualização de grafos feito em C++. O REPL pode ser usado para analises rápidas de propriedades de um grafo.

Para exemplos, assista ao [vídeo](https://www.youtube.com/watch?v=MY5hHPIj3cg).

![](assets/example.gif)

## Dependências para compilação
- CMake 2.6+
- gcc & g++ 5.5+
- SFML 2.4+ (headers)
- TGUI 0.8 (headers)

### Como instalar as dependências?
Um script simples para Ubuntu 18.04+
```bash
sudo add-apt-repository ppa:texus/tgui-0.8
sudo apt-get update
sudo apt-get install libsfml-dev libtgui-dev
```
Ou utilizando o Dockerfile provido no repositório (preferencial).

## Como desenvolver localmente?
```bash
# Boas práticas
mkdir build && cd build
# Gere os scripts de compilação de teste para sua plataforma
cmake ..
# Compilar
make
# Testar
make tests
# Gerar documentação em /docs
make docs
# Executar
make run
# Executar o REPL
make repl
```

## Gerando a documentação

## Como usar o REPL
O REPL possui dois tipos de comando: atribuição e operação.

### Atribuição

#### Atribuição de variável
```
>>> var1 = var2
```
A variável *var1* passa a representar o grafo representado por *var2*.

#### import
```
>>> var = import file_name
```
A variável *var* passa a representar o grafo armazenado em *file_name*. Se a leitura do arquivo falhar, *var* passa a representar um grafo vazio.

#### mst
```
>>> var1 = mst var2
```
A variável *var1* passa a representar a **árvore geradora mínima** do grafo representado por *var2*.

### Operação

#### describe
```
>>> var > describe
```
Exibe informações sobre o grafo representado por *var*.

#### show
```
>>> var > show
```
Exibe o grafo representado por *var*.

#### edit
```
>>> var > edit
```
Exibe o grafo representado por *var*, salvando as modificações efetuadas.

#### reaches
```
>>> var > reaches a b
```
Verifica se o vértice *a* alcança o vértice *b* no grafo representado por *var*.

#### scc
```
>>> var > scc
```
Exibe as componentes fortemente conexas do grafo representado por *var*.

#### shortestPath
```
>>> var > shortestPath a b
```
Computa o peso do caminho mínimo do vértice *a* para o vértice *b* no grafo representado por *var*.

#### coloring
```
>>> var > coloring
```
Computa uma coloração mínima para *var*. O algoritmo é polinomial quando *var* é cordal e **TODO** quando não é.

#### chromaticNumber
```
>>> var > chromaticNumber
```
Computa o número cromático, i.e. o menor número de cores necessárias para se colorir o grafo. O algoritmo é polinomial quando *var* é cordal e **TODO** quando não é.

#### greedyColoring
```
>>> var > greedyColoring
```
Computa uma coloração usando um algoritmo guloso na ordem dos vértices. Linear no tamanho do grafo.

#### maximumCliqueSize
```
>>> var > maximumCliqueSize
```
Computa o tamanho da clique máxima. Disponível apenas para grafos cordais.

#### artPoints
```
>>> var > artPoints
```
Encontra os vértices cujas remoções aumentam o número de componentes conexas do grafo.

#### bridges
```
>>> var > bridges
```
Encontra as arestas cujas remoções aumentam o número de componentes conexas do grafo.

#### topoSort
```
>>> var > topoSort
```
Encontra uma ordenação topológica dos vértices do grafo.

#### center
```
>>> var > center
```
Computa o centro da árvore *var*.

#### diameter
```
>>> var > diameter
```
Computa o diâmetro da árvore *var*.

#### matching
```
>>> var > matching
```
Computa o matching máximo do grafo.

#### tikz
```
>>> var > tikz file [scale]
```
Exporta o grafo em formato tikz para LaTeX no arquivo 'file'. O campo 'scale' é opcional, e altera a escala da imagem.
