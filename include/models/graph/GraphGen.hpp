#ifndef GRAPHGENH
#define GRAPHGENH

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

///
/// @file

///
/// Classe para grafos gerais.
class GraphGen {
  public:
	///
	/// Lista de rótulos dos vértices.
	vector<string> label;

	///
	/// Construtor para o grafo vazio.
	GraphGen();

	///
	/// Construtor para o grafo por número de vértices.
	/// @param Número de vértices.
	GraphGen(int);

	///
	/// Construtor para o grafo por número de vértices e seus rótulos.
	/// @param Número de vértices.
	/// @param Lista de rótulos.
	GraphGen(int, vector<string> &);

	// gets
	///
	/// Retorna o número de vértices
	/// @return Número de vértices do grafo.
	int getN();

	///
	/// Retorn o número de arestas.
	/// @return Número de arestas do grafo.
	int getM();

	///
	/// Retorna a lista de adjacência direcionada.
	/// @return Lista de adjacência direcionada do grafo.
	vector<vector<pair<int, int>>> getAdj();

	///
	/// Retorn a lista de adjacência não direcionada.
	/// @return Lista de adjacêncai não direcionada do grafo.
	vector<vector<pair<int, int>>> getSimAdj();

	///
	/// Retorna a lista das arestas.
	/// @return Lista com as arestas do grafo.
	vector<pair<int, int>> getEdges();

	///
	/// Retorna a lista de pesos das arestas.
	/// @return lista com os pesos do grafo.
	vector<int> getPesos();

	///
	/// Retorna a matriz de adjacência.
	/// @return Matriz de adjacência do grafo.
	vector<vector<int>> getMatrix();

	///
	/// Retorna a matriz de adjacência simétrica.
	/// @return Matriz de adjacência simétrica.
	vector<vector<int>> getSimMatrix();

	// auxiliares
	///
	/// Retorna se o grafo possui arestas com pesos.
	/// @return bool informando se o grafo possui peso nas arestas.
	bool isWeighted();

	///
	/// Retorna se o grafo possui arestas com pesos negativos.
	/// @return bool informando se o grafo possui peso negativo.
	bool hasNegativeWeight();

	///
	/// Retorna se dois vértices se alcançam.
	/// @param Vértice a.
	/// @param Vértice b.
	/// @return bool informando se o vértice 'a' alcança o vértice 'b'.
	bool reaches(int, int);

	///
	/// Retorna se o grafo é conexo.
	/// @return bool informando se o grafo é conexo.
	bool isConnected();

	// reconhecedores
	///
	/// Retorna se o grafo é bipartido.
	/// @return bool informando se o grafo é bipartido.
	bool isBipartite();

	///
	/// Retorna se o grafo é cordal.
	/// @return bool informando se o grafo é cordal.
	bool isChordal();

	///
	/// Retorna se o grafo é direcionado acíclico.
	/// @return bool informando se o grafo é DAG.
	bool isDag();

	///
	/// Retorna se o grafo é uma árvore.
	/// @return bool informando se o grafo é uma árvore.
	bool isTree();

	// algoritmos
	/// Retorna uma lista com a componente fortemente conexa de cada vértice.
	///
	/// Complexidade: O(n + m)
	/// @return Lista tal que na posição 'i', possui o índice da componente
	/// fortemente conexa do vértice 'i'.
	vector<int> scc();

	/// Retorna o menor caminho entre dois vértices.
	///
	/// Complexidade: se todos os pesos forem 1, linear. Se não houver peso
	/// negativo, O(m log(n)). Caso contrário, O(n*m).
	/// @param Vértice 'a'.
	/// @param Vértice 'b'.
	/// @return Comprimento do camínho mínimo de 'a' até 'b'.
	long long shortestPath(int, int);

	// TODO
	/// Retorna uma lista com a cor de cada vértice em uma coloração mínima.
	///
	/// Complexidade: O(nlog(n) + m) quando cordal
	/// @return Lista com a cor de cada vértice.
	virtual vector<int> coloring();

	/// Retorna uma lista com a cor de cada vértice em uma coloração gulosa na
	/// ordem dos vértices.
	///
	/// Complexidade: O(n + m)
	/// @return Lista com a cor de cada vértice.
	vector<int> greedyColoring();

	/// Retorna uma lista com a cor de cada vértice em uma coloração gulosa para
	/// uma dada ordem dos vértices.
	///
	/// Complexidade: O(n + m)
	/// @param Ordem para avaliação do algoritmo guloso.
	/// @return Lista com a cor de cada vértice.
	vector<int> greedyColoring(vector<int>);

	/// Retorna um vetor de booleanos que informa se cada vértice é um ponto de
	/// articulação
	///
	/// Complexidade: O(n + m)
	/// @return Lista informando se cada vértice é ponto de articulação ou não.
	vector<bool> artPoints();

	/// Retorna um vetor de booleanos que informa se cada aresta é uma ponte
	///
	/// Complexidade: O(n + m)
	/// @return Lista com as arestas que são pontes no grafo.
	vector<pair<int, int>> bridges();

	/// Algoritmo Blossom para matching máximo em grafos gerais.
	///
	/// Complexidade: O(n^3)
	/// @return Vetor indicando, para cada vértice, com quem ele está
	/// emparelhado no matching máximo, ou -1 se não estiver emparelhado.
	vector<int> blossom();

	/// Algoritmo para encontrar os autovalores usando
	/// Sylvester's law of inertia. Retorna os autovalores ordenados.
	///
	/// Complexidade: O(n^4 * log(1/EPS))
	/// @Return Vetor com os autovalores ordenados.
	vector<double> getEigenvalues();

  protected:
	///
	/// Número de vértices do grafo.
	int n;

	///
	/// Número de arestas do grafo.
	int m;

	///
	/// Lista de adjacência direcionada.
	vector<vector<pair<int, int>>> adj;

	///
	/// Lista de adjacência não direcionada.
	vector<vector<pair<int, int>>> simAdj;

	/// Retorna uma ordenação dos vértices, e sua ordenação inversa, de máxima
	/// caridinalidade (uma ordenação simplicial se e somente se o grafo for
	/// cordal).
	///
	/// Complexidade: O(nlog(n) + m)
	pair<vector<int>, vector<int>> maximumCardinalitySearch();

	/// Verifica se uma ordenação dos vértices (e sua ordenação inversa) possui
	/// as propriedades de de Zero-Fill-In (i.e., se o grafo é cordal).
	///
	/// Complexidade: O(n + m)
	bool zeroFillIn(pair<vector<int>, vector<int>>);

	/// Verifica se uma ordenação atende às exigências de uma ordenação.
	bool checkOrdering(vector<int>);

  private:
	// auxiliares
	/// Usado para computar se um vértice alcança outro.
	///
	/// Complexidade: O(n+m)
	bool dfsReaches(vector<bool> &, int, int);

	/// Usado para computar se o grafo é conexo.
	///
	/// Complexidade: O(n+m)
	void dfsCheckConnected(vector<bool> &, int);

	/// Usado para computar se um grafo é bipartido.
	///
	/// Complexidade: O(n+m)
	bool dfsCheckBipartite(int, int, vector<int> &);

	/// Usado para computar se um grafo é um dag.
	///
	/// Complexidade: O(n+m)
	bool dfsCheckDag(int, vector<int> &);

	/// Usado para computar se um grafo é uma árvore.
	///
	/// Complexidade: O(n+m)
	bool dfsCheckTree(int, int, vector<int> &);

	/// Usado para computar as componentes fortemente conexas do grafo.
	///
	/// Complexidade: O(n+m)
	int dfsScc(vector<int> &, vector<int> &, vector<int> &, int &, int &, int);

	/// Usado para computar as pontes e pontos de articulação do grafo.
	///
	/// Complexidade: O(n+m)
	void dfsArt(vector<bool> &, vector<int> &, vector<int> &, vector<int> &,
				vector<bool> &, vector<pair<int, int>> &, int, int, int &);

	// algoritmos
	/// BFS para computar caminho mínimo no caso de todos os pesos serem
	/// unitários.
	///
	/// Complexidade: O(n+m)
	long long shortestPathBFS(int, int);

	/// Algoritmo de Dijkstra para caminho mínimo.
	///
	/// Complexidade: O(m log(n))
	long long dijkstra(int, int);

	/// Algoritmo de Bellmand-Ford para caminho mínimo.
	///
	/// Complexidade: O(n*m)
	long long bellmanFord(int, int);

	void contractBlossom(vector<int> &, vector<int> &, vector<int> &,
						 vector<int> &, queue<int> &, int, int, bool = true);

	int getAugmentingPath(vector<int> &, vector<int> &, vector<int> &,
						  vector<int> &, queue<int> &, int);

	int cmpDouble(double, double, double=1e-6);

	vector<int> inertia(vector<vector<double>>);

	void eigenvalueRec(const vector<vector<int>>&, vector<double>&, double, double, int, int);
};

///
/// Classe de exceções de grafos.
class GraphException : public exception {
  public:
	///
	/// Construtor de exceções de grafos.
	GraphException() {}

	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return "Graph Exception"; }
};

///
/// Classe de exceções de ausência de caminho entre dois vértices.
class GraphNoPathException : public GraphException {
  public:
	/// Construtor para a exceção de
	/// ausência de caminho entre dois vértices
	/// @param Vértices que não se alcançam.
	GraphNoPathException(int a, int b) {
		msg = "no path from " + to_string(a) + " to " + to_string(b);
	}

	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return msg.c_str(); }

  private:
	string msg;
};

///
/// Classe de exceções para ciclos negativos em caminhos mínimos.
class GraphNegativeCycleException : public GraphException {
  public:
	///
	/// Construtor para exceção de ciclos negativos.
	GraphNegativeCycleException() {}

	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() {
		return "negative cycle found; shortest path is undefined";
	}
};

///
/// Classe de exceções para ordenações de vértices fora do domínio.
class GraphOrderingException : public GraphException {
  public:
	///
	/// Construtor para exceção de ordenação inválida.
	GraphOrderingException() {}

	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return "not a valid vertex ordering"; }
};

#endif
