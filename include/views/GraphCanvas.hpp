#ifndef GRAPH_CANVASH
#define GRAPH_CANVASH

#include "GraphDisplay.hpp"
#include "Vector.hpp"

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

///
/// @file

///
/// Classe para o canvas.
class GraphCanvas {
  public:
	///
	/// Display do canvas.
	GraphDisplay GD;

	///
	/// Objeto RenderWindow para trabalhar no canvas.
	sf::RenderWindow *janela;

	///
	/// Fonte para textos da interface.
	sf::Font fonte;

	///
	/// Vértice que o usuário está editando.
	int editLabel;

	///
	/// Aresta que o usuário está editando.
	int editWeight;

	///
	/// Construtor vazio
	GraphCanvas() {}

	///
	/// Construtor da classe.
	/// @param Janela.
	/// @param Fonte.
	/// @param X e Y que delimitam o canvas.
	/// @param raio do vértice.
	GraphCanvas(sf::RenderWindow &, sf::Font &, int, int, int = 15);

	///
	/// Define o grafo a ser representado.
	/// @param Grafo a ser representado.
	void setGraph(Graph);

	///
	/// Trata o clique do usuário, descobrindo se ele clicou num vértice,
	/// aresta, etc.
	/// @return bool que informa se algo foi alterado no canvas.
	bool handleClique();

	///
	/// Exibe o grafo.
	void display();

  private:
	///
	/// Retorna uma cor correspondente ao número.
	sf::Color getColor(int);

	///
	/// TODO
	/// Encontra o tamanho apropriado da fonte.
	int findFontSize(int, int);

	///
	/// Mostra os pesos das arestas no canvas.
	void printPesos();

	///
	/// TODO
	/// Mostra as arestas do grafo.
	void printAresta(Vector, int);

	///
	/// TODO
	/// Mostra o grafo no canvas.
	void printGrafo();

	///
	/// Mostra as setas nas arestas direcionadas.
	void printSetas();
};

///
/// Retorna grafo lido de um dado arquivo.
Graph lerGrafoArquivo(string);

///
/// Classe de exceções para leitura de grafos inexistentes.
class FileNotFoundException : public exception {
  public:
	///
	/// Construtor da exceção.
	/// @param Nome do arquivo.
	FileNotFoundException(string f) { msg = "coudn't read file: " + f; }

	///
	/// Retorna a mensagem de erro da exceção.
	/// @return Mensagem de erro.
	const char *what() const throw() { return msg.c_str(); }

  private:
	/// Mensagem de erro da exceção.
	string msg;
};

#endif
