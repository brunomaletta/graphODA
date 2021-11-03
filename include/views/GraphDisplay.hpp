#ifndef GRAPH_DISPLAYH
#define GRAPH_DISPLAYH

#include "Graph.hpp"
#include "Vector.hpp"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

///
/// @file

///
/// Classe para display do grafo.
class GraphDisplay {
  public:
	///
	/// Grafo que está sendo representado.
	Graph G;

	///
	/// Variável que indica se o grafo deve ser representado de forma
	/// direcionada.
	bool temDir;

	///
	/// Variável que indica se está no modo 'centralizar'.
	bool centr;

	///
	/// Variável que indica se o display está no modo 'draw'.
	bool draw;

	///
	/// Tamanho do raio da representação de cada vértice.
	int raio;

	///
	/// Tamanho do eixo horizontal do canvas.
	int X;

	///
	/// Tamanho do eixo vertical do canvas.
	int Y;

	///
	/// Lista que indica se cada vértice deve ser parado (quando o usuário
	/// clica nele).
	vector<bool> para;

	///
	/// Lista que indica se cada vértice está travado.
	vector<bool> trava;

	///
	/// Lista contendo a posição de cada vértice.
	vector<Vector> pos;

	///
	/// Lista contendo a velocidade de cada vértice.
	vector<Vector> vel;

	///
	/// Lista contendo a posição de cada peso em relação à sua aresta.
	///
	/// Valores vão de 0 a 1, de forma que 0.5 indica que o peso está
	/// no meio da aresta.
	vector<float> posPeso;

	///
	/// Lista contendo a cor de cada vértice.
	vector<int> color;

	///
	/// Lista contendo a cor de cada aresta.
	vector<int> colorAresta;

	///
	/// Lista que indica se cada aresta é 'paralela' ou não.
	vector<bool> isParal;

	///
	/// Matriz adjacencia simétrica
	vector<vector<int>> simMatrix;

	///
	/// Autovalores
	vector<double> eigenvalues;

	///
	/// Construtor de GraphDisplay vazio.
	GraphDisplay() {}

	///
	/// Construtor de GraphDisplay com base no seu grafo, nas dimensões do
	/// canvas e no tamanho do raio do vértice.
	/// @param Grafo a ser representado.
	/// @param X e Y que delimitam o canvas.
	/// @param Raio do vértice.
	GraphDisplay(Graph, int, int, int = 15);

	///
	/// Seta um grafo no display.
	/// @param Grafo a ser representado.
	void setGraph(Graph &);

	///
	/// Seta um grafo no display, mantendo as confugurações do grafo anterior.
	/// @param Grafo a ser representado.
	void setGraphContinue(Graph &);

	///
	/// Algoritmo de display para os pesos.
	/// @param Número de iterações do algoritmo.
	void fdpPeso(int);

	///
	/// Garante que um vértice está dentro do canvas.
	/// @param Posição atual do vértice.
	/// @param bool indicando se o vértice está travado.
	/// @return Nova posição do vértice.
	Vector deixaDentro(Vector, bool);

	///
	/// Verifica se uma posição está dentrod do canvas.
	/// @param Posição do vértice.
	/// @return bool que indica se o vértice está dentro do canvas, ou não.
	bool taDentro(Vector);

	///
	/// ALgoritmo de display de Eades.
	/// @param Número de iterações do algoritmo.
	void fdpEades(int);

	///
	/// Algoritmo de display de Eades usando aceleração.
	/// @param Número de iterações do algoritmo.
	void fdpEadesAcc(int);

	///
	/// Algoritmo de display de Fruchterman.
	/// @param Número de iterações do algoritmo.
	void fdpFruchterman(int);

	///
	/// Computa número de interseções de arestas no display atual.
	/// @return Número de interseções de arestas no display atual.
	int inter();

	///
	/// Algoritmo display polígono.
	void poligono();

	///
	/// Display aleatório
	void random();

	///
	/// Tenta encontrar um display visualmente agradável.
	/// @param Número de tentativas de posições iniciais dos vértices.
	/// @param Número de iterações do algoritmo para encontrar um display
	/// agradável.
	void good(int, int);

	///
	/// Encontra vértice em dada posição.
	/// @param Posição a ser consultada.
	/// @return Vértice em tal posição, ou -1 se não existir vértice em tal
	/// posição.
	int achaVertice(Vector);

	///
	/// Encontra aresta em dada posição.
	/// @param Posição a ser consultada.
	/// @return Aresta em tal posição, ou -1 se não existir aresta em tal
	/// posição.
	int achaAresta(Vector);

	///
	/// Reseta as cores dos vértices e arestas.
	void resetColors();

	///
	/// Adiciona um vértice em dada posição.
	/// @param Posição a se adicionar um novo vértice
	void addVertex(Vector);

	///
	/// Remove um dado vértice.
	/// @param Vértice a ser removido.
	void removeVertex(int);

	///
	/// Adiciona uma aresta entre dois vértices.
	/// @param Vértices que definem a aresta a ser adicionada.
	void addEdge(int, int);

	///
	/// Reomove uma dada aresta.
	/// @param Aresta a ser removida.
	void removeEdge(int);

	///
	/// Itera a animação do display.
	void itera();

	///
	/// Gera código LaTeX (tikz) para o grafo.
	/// @param Nome do arquivo onde salvar o código LaTeX.
	/// @param Escala da imagem.
	void getTikz(string, double);
};

#endif
