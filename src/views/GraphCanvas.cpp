#include "GraphCanvas.hpp"

GraphCanvas::GraphCanvas(sf::RenderWindow &janela, sf::Font &fonte, int X,
						 int Y, int raio) {
	this->janela = &janela;
	this->fonte = fonte;
	this->GD = GraphDisplay(Graph(), X, Y, raio);
	editLabel = -1;
	editWeight = -1;
	drawLabels = true;
}

int getIntSize(int n) {
	int s = 1;
	while (n /= 10) s++;
	return s;
}

int GraphCanvas::findFontSize(int n, int fontSize) {
	if (n < 2)
		return fontSize;
	else if (n < 3)
		return fontSize - 6;
	else if (n < 4)
		return fontSize - 10;
	else if (n < 5)
		return fontSize - 14;
	else
		return fontSize - 10;
}

sf::Color GraphCanvas::getColor(int x) {
	if (x == 0) return sf::Color::White;
	if (x == 1) return sf::Color(134, 194, 50);  // green
	if (x == 2) return sf::Color(255, 34, 12);   // red
	if (x == 3) return sf::Color(136, 217, 230); // blue
	if (x == 4) return sf::Color(254, 215, 102); // yeallow
	if (x == 5) return sf::Color(244, 96, 54);   // orange
	if (x == 6) return sf::Color(160, 108, 213); // purple
	if (x == 7) return sf::Color(235, 186, 185); // light pink
	if (x == 8) return sf::Color(231, 226, 71);  // light green
	if (x == 9) return sf::Color(60, 145, 230);  // mid blue
	if (x == 10) return sf::Color(168, 118, 62); // beje
	if (x == 11) return sf::Color(50, 84, 115);  // dark blue
	return sf::Color::Black;
}

void GraphCanvas::printAresta(Vector at, int aresta) {
	auto position = sf::Mouse::getPosition(*janela);
	Vector positionV(position.x, position.y);
	positionV = GD.deixaDentro(positionV, 0);

	sf::Vertex linha[] = {
		sf::Vertex(sf::Vector2f(positionV.x, positionV.y), sf::Color::Black),

		sf::Vertex(sf::Vector2f(GD.pos[aresta].x, GD.pos[aresta].y),
				   sf::Color::Black)};

	janela->draw(linha, 2, sf::Lines);

	if (GD.temDir) {
		const float pi = acos(-1.0);

		Vector fim = positionV;
		Vector v = fim - GD.pos[aresta];
		Vector unit = v * (1 / v.norm());

		Vector pos = fim - unit * GD.raio * 0.5;

		// angulo que tem que rodar pra seta ficar certa
		float angle = v.angle() - pi / 6;

		Vector delta = Vector(GD.raio / 2 - 1, GD.raio / 2 - 1).rotate(angle) -
					   Vector(GD.raio / 2 - 1, GD.raio / 2 - 1);

		// arreda um pouqinho a seta se ta dentro do vertice
		float add = 0;
		int achaAt = GD.achaVertice(at);
		if (achaAt > -1 and achaAt != aresta) {
			Vector yv = GD.pos[GD.achaVertice(at)] - at;
			float y = yv.norm();

			float alpha = (GD.pos[aresta] - at).angle() - yv.angle();

			// lei dos cossenos mt louca
			add = y * cos(alpha) +
				  sqrt(GD.raio * GD.raio - y * y * sin(alpha) * sin(alpha));
		}

		Vector addV = v * (1 / v.norm()) * (-add);

		// cria triangulo na ponta da aresta
		sf::CircleShape tri(GD.raio / 2, 3);
		tri.setRotation((angle)*180 / pi);
		tri.setFillColor(sf::Color::Black);
		tri.setPosition(pos.x - GD.raio / 2 + 1 - delta.x + addV.x,
						pos.y - GD.raio / 2 + 1 - delta.y + addV.y);
		janela->draw(tri);
	}
}

void GraphCanvas::printGrafo() {
	auto edg = GD.G.getEdges();

	// Cria as arestas
	for (int i = 0; i < GD.G.getM(); i++) {
		// Arestas sem largura, por isso vetores
		Vector add(0, 0);
		if (GD.isParal[i] and GD.temDir) {
			add = GD.pos[edg[i].second] - GD.pos[edg[i].first];
			if (add.norm()) {
				add = add * (1 / add.norm());
				add = add.rotate(acos(-1.0) / 2);
				add = add * (GD.raio / 3.0);
			}
		}

		sf::Vector2f ini(GD.pos[edg[i].first].x + add.x,
						 GD.pos[edg[i].first].y + add.y);
		sf::Vector2f fim(GD.pos[edg[i].second].x + add.x,
						 GD.pos[edg[i].second].y + add.y);

		sf::Vector2f dif = fim - ini;
		Vector difV(dif.x, dif.y);

		sf::RectangleShape aresta(sf::Vector2f(difV.norm(), 0));
		if (GD.temDir)
			aresta = sf::RectangleShape(
				sf::Vector2f(difV.norm() - 1.5 * GD.raio, 0));
		aresta.rotate(180 * difV.angle() / acos(-1.0));
		aresta.move(ini);
		aresta.setOutlineThickness(2);
		aresta.setOutlineColor(getColor(GD.colorAresta[i]));

		janela->draw(aresta);
	}

	// encontra tamanho da fonte
	int fontSize = 10000;
	for (auto &i : GD.G.label)
		fontSize = min(fontSize, findFontSize(i.size(), 24));

	// Cria os vértices
	for (int i = 0; i < GD.G.getN(); i++) {
		// Cria um círculo
		sf::CircleShape v(GD.raio);
		v.setFillColor(getColor(GD.color[i]));

		// vertice travado
		if (GD.trava[i])
			v.setOutlineThickness(4.f);
		else
			v.setOutlineThickness(2.f);

		v.setOutlineColor(sf::Color::Black);
		v.setPosition(GD.pos[i].x - GD.raio + 1, GD.pos[i].y - GD.raio + 1);
		janela->draw(v);

		if (drawLabels) {
			// Coloca texto dentro da bola
			sf::Text label;
			label.setFont(fonte);
			label.setString(GD.G.label[i]);
			label.setCharacterSize(fontSize);
			label.setFillColor(sf::Color::Black);

			// centraliza texto
			sf::FloatRect box = label.getLocalBounds();
			label.setOrigin(box.left + round(box.width / 2),
							box.top + round(box.height / 2));
			label.setPosition(GD.pos[i].x, GD.pos[i].y);

			if (i != editLabel) janela->draw(label);
		}
	}
}

// eu sou mt bom com vetores
void GraphCanvas::printSetas() {
	const float pi = acos(-1.0);
	auto edg = GD.G.getEdges();

	for (int i = 0; i < GD.G.getM(); i++) {
		Vector fim = GD.pos[edg[i].second];
		Vector v = fim - GD.pos[edg[i].first];
		Vector unit = v * (1 / v.norm());

		Vector pos = fim - unit * GD.raio * 1.6;
		// se ta travado arreda um pouco a seta
		if (GD.trava[edg[i].second]) pos = pos - unit * 2;

		// angulo que tem que rodar pra seta ficar certa
		float angle = v.angle() - pi / 6;

		Vector delta = Vector(GD.raio / 2 - 1, GD.raio / 2 - 1).rotate(angle) -
					   Vector(GD.raio / 2 - 1, GD.raio / 2 - 1);

		// trata aresta paralelas
		Vector add(0, 0);
		if (GD.isParal[i]) {
			add = GD.pos[edg[i].second] - GD.pos[edg[i].first];
			if (add.norm()) {
				add = add * (1 / add.norm());
				add = add.rotate(acos(-1.0) / 2);
				add = add * (GD.raio / 3.0);
			}
		}

		// cria triangulo na ponta da aresta
		sf::CircleShape tri(GD.raio / 2, 3);
		tri.setRotation((angle)*180 / pi);
		tri.setFillColor(getColor(GD.colorAresta[i]));
		tri.setPosition(pos.x - GD.raio / 2 + 1 - delta.x + add.x,
						pos.y - GD.raio / 2 + 1 - delta.y + add.y);
		janela->draw(tri);
	}
}

void GraphCanvas::printPesos() {
	int fontSize = 10000;
	auto adj = GD.G.getAdj();
	for (int i = 0; i < GD.G.getN(); i++)
		for (auto j : adj[i])
			fontSize = min(fontSize, findFontSize(getIntSize(j.second), 24));
	auto edg = GD.G.getEdges();
	auto pesos = GD.G.getPesos();

	for (int i = 0; i < GD.G.getM(); i++) {
		sf::Text p;
		p.setFont(fonte);
		p.setString(to_string(pesos[i]));
		p.setCharacterSize(fontSize);
		p.setFillColor(sf::Color::Black);

		// acha posicao
		Vector v = GD.pos[edg[i].second] - GD.pos[edg[i].first];
		Vector at = GD.pos[edg[i].first] + v * GD.posPeso[i];

		// arestas paralelas
		Vector add(0, 0);
		if (GD.isParal[i]) {
			add = GD.pos[edg[i].second] - GD.pos[edg[i].first];
			if (add.norm()) {
				add = add * (1 / add.norm());
				add = add.rotate(acos(-1.0) / 2);
				add = add * (GD.raio / 3.0);
			}
		}

		sf::FloatRect box = p.getLocalBounds();
		p.setOrigin(box.left + round(box.width / 2),
					box.top + round(box.height / 2));
		p.setPosition(at.x + add.x, at.y + add.y);
		p.setOutlineColor(sf::Color::White);
		p.setOutlineThickness(3.0);

		if (i != editWeight) janela->draw(p);
	}
}

Graph lerGrafoArquivo(string arq) {
	ifstream inFile(arq);
	if (!inFile) {
		inFile = ifstream("grafos/grafo" + arq + ".txt");
		if (!inFile) throw FileNotFoundException(arq);
	}

	int n, m;
	inFile >> n >> m;
	vector<string> label(n);
	for (auto &i : label) inFile >> i;

	Graph G(n, label);

	// verfica se tem peso ou nao
	string l;
	inFile.ignore();
	getline(inFile, l);
	stringstream ss(l);
	vector<int> entrada;
	for (int i; ss >> i;) entrada.push_back(i);
	bool temPeso = false;
	if (entrada.size() == 3) {
		temPeso = true;
		G.addEdge(entrada[0], entrada[1], entrada[2]);
	} else
		G.addEdge(entrada[0], entrada[1]);

	for (int i = 0; i < m - 1; i++) {
		int a, b;
		inFile >> a >> b;
		if (temPeso) {
			int p;
			inFile >> p;
			G.addEdge(a, b, p);
		} else
			G.addEdge(a, b);
	}

	inFile.close();

	return G;
}

void GraphCanvas::setGraph(Graph G) {
	bool temDir = GD.temDir;
	bool draw = GD.draw;
	GD.setGraph(G);
	GD.temDir = temDir;
	GD.draw = draw;
	GD.good(max(10, 100 - GD.G.getN()), max(10, 100 - GD.G.getM()));
}

// muitos ifs
bool GraphCanvas::handleClique() {
	// variaveis estaticas para controle
	// (nao arranjei forma melhor)
	static bool clique = 0, cliqueD = 0;
	static Vector ini, dif;
	static int vertice, aresta; // vertice clicado
	static int lastVertice = -1, lastAresta = -1;
	static bool crieiVertice = 0;
	static sf::Clock clock;
	static sf::Time lastTime = clock.getElapsedTime(); // ultima vez que clicou

	bool mudou = false;

	auto position = sf::Mouse::getPosition(*janela);
	Vector positionV(position.x, position.y);

	// atualiza vertice
	if (!(vertice > -1 and clique)) vertice = GD.achaVertice(positionV);

	// desenha / destroi aresta pela metade
	if (!GD.taDentro(positionV)) lastVertice = -1;
	if (lastVertice > -1) printAresta(positionV, lastVertice);

	// cliquei com o botao esquerdo
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (!clique) {
			clique = 1;
			ini = positionV;
			if (vertice > -1) dif = GD.pos[vertice] - positionV;

			if (GD.draw) {
				// add vertice
				if (vertice == -1 and GD.taDentro(positionV) and
					GD.achaAresta(positionV) == -1) {
					GD.addVertex(positionV);
					mudou = true;
					crieiVertice = 1;
					dif = Vector(0, 0);
				}
			} else {
				// trava vertice
				if (vertice > -1) {
					GD.centr = 0;
					GD.para[vertice] = 1;
				}
			}
		}

		// move vertice
		if (vertice > -1)
			GD.pos[vertice] =
				GD.deixaDentro(positionV + dif, GD.trava[vertice]);
	} else if (clique) { // soltei
		clique = 0;
		lastAresta = aresta;
		aresta = GD.achaAresta(positionV);

		// cliquei e soltei no mesmo lugar
		if (abs(ini.x - positionV.x) <= 2 and abs(ini.y - positionV.y) <= 2) {
			// testa se foi um double click
			sf::Time atual = clock.getElapsedTime();
			float delta = atual.asSeconds() - lastTime.asSeconds();
			lastTime = atual;
			if (delta <= 0.5 and GD.draw) {
				if (vertice > -1 and vertice == lastVertice) {
					editLabel = vertice;
					lastVertice = -1;
					return mudou;
				} else if (aresta > -1 and aresta == lastAresta) {
					editWeight = aresta;
					lastAresta = -1;
					return mudou;
				}
			}

			if (vertice > -1) {
				if (!GD.draw) { // trava
					GD.trava[vertice] = !GD.trava[vertice];
					GD.para[vertice] = 0;
				} else { // desenha aresta
					if (lastVertice == -1 and !crieiVertice)
						lastVertice = vertice;
					else {
						if (lastVertice > -1 and lastVertice != vertice) {
							GD.addEdge(lastVertice, vertice);
							aresta = -1;
							mudou = true;
						}
						lastVertice = -1;
					}
				}
			} else
				lastVertice = -1;
		} else if (vertice > -1)
			GD.para[vertice] = 0; // solta vertice

		crieiVertice = 0;
	}

	// cliquei com o botao direito
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		if (!cliqueD) {
			cliqueD = 1;

			if (GD.draw) {
				if (vertice > -1) {
					// remove vertice
					if (lastVertice == vertice) lastVertice = -1;
					GD.removeVertex(vertice);
					mudou = true;
				} else {
					// remove aresta
					int arestaClicada = GD.achaAresta(positionV);
					if (arestaClicada > -1) {
						GD.removeEdge(arestaClicada);
						mudou = true;
					}
				}
			}
		}
	} else if (cliqueD)
		cliqueD = 0;

	return mudou;
}

void GraphCanvas::display() {
	GD.itera();
	if (GD.temDir) printSetas();
	printGrafo();
	if (GD.G.isWeighted()) printPesos();
}
