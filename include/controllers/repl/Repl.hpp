#ifndef REPLH
#define REPLH

#include "Bipartite.hpp"
#include "Chordal.hpp"
#include "Dag.hpp"
#include "Graph.hpp"
#include "GraphCanvas.hpp"
#include "Interface.hpp"
#include "Tree.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

///
/// @file

namespace repl {
class TYPE {
  public:
	Graph G;
	Bipartite B;
	Chordal C;
	Dag D;
	Tree T;

	int type;
};

extern vector<TYPE> graphs;
extern map<string, int> vars;
extern stringstream ss;
extern string var;
extern int at;
extern int lastVar;

// aux
string getType(int);
Graph getGraphVar(string);
Graph getGraph();
int getVertex(Graph &, string);
void undoDeclaration();

// errors
void fileNotFoundError(string);
void undefinedVariableError(string);
void wrongTypeError(string, int);
void graphIsNotError(string, int);
void vertexNotFoundError(string);
void noPathError(string, string);
void negativeCycleError();

// commands
void import();
void mst();
void graphCast();
void bipartiteCast();
void chordalCast();
void dagCast();
void treeCast();
void show(int, int);
void edit(int, int);
void describe();
void reaches();
void scc();
void shortestPath();
void coloring();
void chromaticNumber();
void greedyColoring();
void maxClique();
void artPoints();
void bridges();
void topoSort();
void center();
void diameter();
void matching();
void tikz(int, int);

void run(int, int);
} // namespace repl

///
/// Classe para exceções do REPL.
class REPLException : public exception {
  public:
	///
	/// Construtor de exceções do REPL.
	REPLException() {}
	///
	/// Retorna mensagem de erro da exceção.
	/// @return Mensagem de erro;.
	const char *what() const throw() { return "REPL Exception"; }
};

///
/// Classe de exceções de comando inválido.
class REPLInvalidCommandException : public REPLException {
  public:
	///
	/// Construtor de exceções de comando inválido.
	REPLInvalidCommandException() {}
	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return "invalid syntax"; }
};

///
/// Classe de exceções de vértice não encontrado.
class REPLVertexNotFoundException : public REPLException {
  public:
	///
	/// Construtor de exceções de vértice não encontrado.
	/// @param string v que é o label de um vértice.
	REPLVertexNotFoundException(string v) {
		msg = "coudn't find vertex with label: " + v;
	}
	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return msg.c_str(); }

  private:
	string msg;
};

#endif
