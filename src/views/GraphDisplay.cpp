#include "GraphDisplay.hpp"

#define EPS 1e-6

GraphDisplay::GraphDisplay(Graph G_, int X_, int Y_, int raio_) {
	G = G_;
	X = X_;
	Y = Y_;
	raio = raio_;
	temDir = centr = draw = 0;
	this->poligono();
	vel = vector<Vector>(G.getN(), Vector(0, 0));
	para = vector<bool>(G.getN(), false);
	trava = vector<bool>(G.getN(), false);
	color = vector<int>(G.getN(), 0);
	colorAresta = vector<int>(G.getM(), 100);
	posPeso = vector<float>(G.getM(), 0.5);

	isParal = vector<bool>(G.getM(), false);
	auto adj = G.getMatrix();
	auto edg = G.getEdges();
	for (int i = 0; i < G.getM(); i++)
		if (adj[edg[i].second][edg[i].first]) isParal[i] = true;
}

void GraphDisplay::setGraph(Graph &G) { *this = GraphDisplay(G, X, Y, raio); }

void GraphDisplay::setGraphContinue(Graph &G) { this->G = G; }

// encontra vertice na posicao 'at'
int GraphDisplay::achaVertice(Vector at) {
	for (int i = G.getN() - 1; i >= 0; i--)
		if ((at ^ pos[i]) < raio) return i;
	return -1;
}

// encontra aresta na posicao 'at'
int GraphDisplay::achaAresta(Vector at) {
	auto edg = G.getEdges();
	float best = 1000000;
	int ar = -1;
	for (int i = 0; i < G.getM(); i++) {
		Vector ini = pos[edg[i].first], fim = pos[edg[i].second];

		Vector add(0, 0);
		if (isParal[i]) {
			add = fim - ini;
			if (add.norm()) {
				add = add * (1 / add.norm());
				add = add.rotate(acos(-1.0) / 2);
				add = add * (raio / 3.0);
			}
		}

		ini = ini + add;
		fim = fim + add;

		float d = (at - ini).norm() + (fim - at).norm();
		if (d - 0.5 < (fim - ini).norm() and d < best) {
			best = d;
			ar = i;
		}
	}
	return ar;
}

// O(it * m^2)
void GraphDisplay::fdpPeso(int it) {
	// constroi posicoes de fato
	vector<Vector> posPesoV;
	auto edg = G.getEdges();
	for (int i = 0; i < G.getM(); i++) {
		Vector v = pos[edg[i].second] - pos[edg[i].first];
		posPesoV.push_back(pos[edg[i].first] + v * posPeso[i]);
	}

	// constantes do algoritmo de Eades
	float c3 = 50000 / (float)(1 + G.getN() + G.getM()), c4 = 0.1;

	while (it--) {
		// forca aplicada a cada peso
		vector<Vector> forca;

		for (int i = 0; i < G.getM(); i++) {
			Vector f(0, 0);

			// forca em relacao aos vertices adjacentes
			for (int j = 0; j < 2; j++) {
				// escolhe um deles
				Vector v = pos[edg[i].first];
				if (j) v = pos[edg[i].second];

				float d = posPesoV[i] ^ v;
				if (d < EPS) d = EPS;

				// vetor unitario na direcao de i para j
				Vector unit = (v - posPesoV[i]) * (1 / d);

				// repulsao
				f = f - unit * (c3 / (d * d));
			}

			// forca em relacao aos pesos
			for (int j = 0; j < G.getM(); j++)
				if (i != j) {
					float d = posPesoV[i] ^ posPesoV[j];
					if (d < EPS) d = EPS;
					Vector unit = (posPesoV[j] - posPesoV[i]) * (1 / d);
					f = f - unit * (c3 / (d * d));
				}

			// projeta a forca no vetor da aresta
			Vector ar = pos[edg[i].second] - pos[edg[i].first];
			f = f.project(ar);
			forca.push_back(f);
		}

		// atualiza posicoes
		for (int i = 0; i < G.getM(); i++)
			posPesoV[i] = posPesoV[i] + forca[i] * c4;
	}

	// transforma de volta para numeros
	posPeso = vector<float>();
	for (int i = 0; i < G.getM(); i++) {
		Vector v = pos[edg[i].second] - pos[edg[i].first];
		if (v.norm() < EPS) {
			posPeso.push_back(0.5);
			continue;
		}
		posPeso.push_back((posPesoV[i] - pos[edg[i].first]).norm() / v.norm());

		// para o peso ficar na aresta
		if (posPeso[i] < 0.1) posPeso[i] = 0.1;
		if (posPeso[i] > 0.9) posPeso[i] = 0.9;
	}
}

Vector GraphDisplay::deixaDentro(Vector v, bool trav) {
	v.x = max(v.x, (float)raio + 1 + 2 * trav);
	v.y = max(v.y, (float)raio + 1 + 2 * trav);
	v.x = min(v.x, (float)X - raio - 3 - 2 * trav);
	v.y = min(v.y, (float)Y - raio - 3 - 2 * trav);
	return v;
}

bool GraphDisplay::taDentro(Vector v) {
	if (v.x < 0) return 0;
	if (v.x > X) return 0;
	if (v.y < 0) return 0;
	if (v.y > Y) return 0;
	return 1;
}

// Eades algorithm ; O(it * n^2)
void GraphDisplay::fdpEades(int it) {
	auto adj = G.getSimMatrix();

	// constantes do algoritmo
	float c1 = 20, c2 = 100, c3 = 50000, c4 = 0.1, c5 = 10000;

	// numero de iteracoes
	while (it--) {
		// forca aplicada a cada vertice
		vector<Vector> forca;

		for (int i = 0; i < G.getN(); i++) {
			Vector f(0, 0);

			for (int j = 0; j < G.getN(); j++)
				if (j != i) {
					float d = pos[i] ^ pos[j];
					if (d < EPS) d = EPS;

					// vetor unitario na direcao de i para j
					Vector unit = (pos[j] - pos[i]) * (1 / d);

					// computa forca de acordo com o algoritmo
					if (!adj[i][j])
						f = f - unit * (c3 / (d * d));
					else
						f = f +
							unit * (c1 * log(d / (c2 + G.getN() + G.getM())));
				}

			// forca em relacao as paredes
			vector<Vector> parede = {Vector(0, pos[i].y), Vector(X, pos[i].y),
									 Vector(pos[i].x, 0), Vector(pos[i].x, Y)};
			for (auto j : parede) {
				float d = pos[i] ^ j;
				if (d < EPS) d = EPS;
				Vector unit = (j - pos[i]) * (1 / d);
				f = f - unit * (c5 / (d * d));
			}

			forca.push_back(f);
		}

		// atualiza posicoes
		for (int i = 0; i < G.getN(); i++)
			if (!para[i] and !trava[i])
				pos[i] = deixaDentro(pos[i] + forca[i] * c4, trava[i]);
	}
}

// Eades algorithm with acceleration ; O(it * n^2)
void GraphDisplay::fdpEadesAcc(int it) {
	auto adj = G.getSimMatrix();

	// constantes do algoritmo
	float c1 = 20, c2 = 100, c3 = 50000, c4 = 0.1, c5 = 10000;

	// numero de iteracoes
	while (it--) {
		// forca aplicada a cada vertice
		vector<Vector> forca;

		for (int i = 0; i < G.getN(); i++) {
			Vector f(0, 0);

			for (int j = 0; j < G.getN(); j++)
				if (j != i) {
					float d = pos[i] ^ pos[j];
					if (d < EPS) d = EPS;

					// vetor unitario na direcao de i para j
					Vector unit = (pos[j] - pos[i]) * (1 / d);

					// computa forca de acordo com o algoritmo
					if (!adj[i][j])
						f = f - unit * (c3 / (d * d));
					else
						f = f +
							unit * (c1 * log(d / (c2 + G.getN() + G.getM())));
				}

			// forca em relacao as paredes
			vector<Vector> parede = {Vector(0, pos[i].y), Vector(X, pos[i].y),
									 Vector(pos[i].x, 0), Vector(pos[i].x, Y)};
			for (auto j : parede) {
				float d = pos[i] ^ j;
				if (d < EPS) d = EPS;
				Vector unit = (j - pos[i]) * (1 / d);
				f = f - unit * (c5 / (d * d));
			}

			// forca dissipativa
			Vector dis = vel[i] * (-0.2);
			f = f + dis;

			forca.push_back(f);
		}

		// atualiza velocidade
		for (int i = 0; i < G.getN(); i++) vel[i] = vel[i] + forca[i] * c4;
		for (int i = 0; i < G.getN(); i++)
			if (para[i] or trava[i]) vel[i] = Vector(0, 0);

		// atualiza posicoes
		for (int i = 0; i < G.getN(); i++)
			pos[i] = deixaDentro(pos[i] + vel[i], trava[i]);
	}
}

// Fruchterman algorithm ; O(it * n^2)
void GraphDisplay::fdpFruchterman(int it) {
	auto adj = G.getSimMatrix();

	// constante do algoritmo
	float k = sqrt(X * Y / float(G.getN())) / 2;
	float t = min(X, Y) / 8.0;
	float delta = t / it;

	while (it--) {
		// forca aplicada a cada vertice
		vector<Vector> forca;

		for (int i = 0; i < G.getN(); i++) {
			Vector f(0, 0);

			for (int j = 0; j < G.getN(); j++)
				if (j != i) {
					float d = pos[i] ^ pos[j];
					if (d < EPS) d = EPS;

					// vetor unitario na direcao de i para j
					Vector unit = (pos[j] - pos[i]) * (1 / d);

					// computa forca de acordo com o algoritmo
					if (d < 2 * k) f = f - unit * (k * k / d);
					if (adj[i][j]) f = f + unit * (d * d / k);
				}

			// limitante das forcas
			float disp = f.norm();
			if (disp > EPS) {
				f = f * (1 / disp);
				f = f * min(disp, t);
			}

			forca.push_back(f);
		}

		// atualiza posicoes
		for (int i = 0; i < G.getN(); i++) pos[i] = pos[i] + forca[i];

		t = max((float)0, t - delta);
	}
}

// se os segmentos de reta interceptam
bool cruza(Vector a, Vector b, Vector c, Vector d) {
	if (((b - a) % (c - b)) * ((b - a) % (d - b)) > 0) return 0;
	if (((d - c) % (a - d)) * ((d - c) % (b - d)) > 0) return 0;
	return 1;
}

// numero de intersecoes de arestas ; O(m^2)
int GraphDisplay::inter() {
	int ret = 0;
	auto edg = G.getEdges();
	for (auto &i : edg)
		for (auto &j : edg)
			if (cruza(pos[i.first], pos[i.second], pos[j.first], pos[j.second]))
				ret++;
	return ret;
}

// O(n)
void GraphDisplay::poligono() {
	const float raioGrafo = (min(X, Y) - 100) / 2;
	Vector centro(X / 2, Y / 2);
	// angulo = 360/n
	const double theta = 2 * acos(-1.0) / G.getN();

	// numero impar de vertices tem que arredar um pouco
	if (G.getN() % 2 == 1) {
		float falta = raioGrafo - raioGrafo * cos(theta / 2);
		centro = centro + Vector(0, falta / 2);
	}

	pos = vector<Vector>();
	for (int i = 0; i < G.getN(); i++)
		pos.push_back(
			Vector(centro.x + sin(i * theta + theta / 2) * raioGrafo,
				   centro.y + cos(i * theta + theta / 2) * raioGrafo));
}

// O(n)
void GraphDisplay::random() {
	pos = vector<Vector>();
	for (int i = 0; i < G.getN(); i++)
		pos.push_back(
			Vector((rand() % (X - 100)) + 50, (rand() % (Y - 100) + 50)));
}

// O(randIt * fdpIt * n^2)
void GraphDisplay::good(int randIt, int fdpIt) {
	int best = 2 * G.getM() * G.getM() + 10;
	vector<Vector> posBest = vector<Vector>();
	for (int i = 0; i < randIt; i++) {
		this->random();
		this->fdpFruchterman(fdpIt);

		int inters = this->inter();
		if (inters < best) {
			best = inters;
			posBest = this->pos;
		}
	}
	this->pos = posBest;
	this->fdpEades(randIt * fdpIt / 4);
	this->fdpEadesAcc(randIt * fdpIt / 4);
	this->fdpPeso(randIt * fdpIt / 4);
}

void GraphDisplay::resetColors() {
	color = vector<int>(G.getN(), 0);
	colorAresta = vector<int>(G.getM(), 100);
}

// O(n^2)
void GraphDisplay::addVertex(Vector v) {
	int newLabel = 0;
	while (1) {
		bool isValid = 1;
		for (auto &i : G.label)
			if (i == to_string(newLabel)) isValid = 0;
		if (isValid) break;
		newLabel++;
	}
	G.label.push_back(to_string(newLabel));
	Graph G2(G.getN() + 1, G.label);
	pos.push_back(v);
	vel.push_back(Vector(0, 0));
	para.push_back(0);
	trava.push_back(0);
	color.push_back(0);
	auto adj = G.getAdj();
	for (int i = 0; i < G.getN(); i++)
		for (auto j : adj[i]) G2.addEdge(i, j.first, j.second);
	G = G2;
	this->resetColors();
}

// O(n+m)
void GraphDisplay::removeVertex(int v) {
	G.label.erase(G.label.begin() + v);
	Graph G2(G.getN() - 1, G.label);
	pos.erase(pos.begin() + v);
	vel.erase(vel.begin() + v);
	para.erase(para.begin() + v);
	trava.erase(trava.begin() + v);
	color.erase(color.begin() + v);

	int erased = 0, position = -1;
	auto adj = G.getAdj();
	for (int i = 0; i < G.getN(); i++)
		for (auto j : adj[i]) {
			position++;
			if (i == v or j.first == v) {
				posPeso.erase(posPeso.begin() + position - erased);
				isParal.erase(isParal.begin() + position - erased);
				erased++;
				continue;
			}
			int a = i, b = j.first;
			if (a > v) a--;
			if (b > v) b--;
			G2.addEdge(a, b, j.second);
		}
	G = G2;
	this->resetColors();
}

// O(n+m)
void GraphDisplay::addEdge(int a, int b) {
	auto adj = G.getAdj();
	for (auto i : adj[a])
		if (i.first == b) return;
	G.addEdge(a, b);
	int posicao = 0;
	bool achou = false;
	adj = G.getAdj();
	for (int i = 0; i < G.getN() and !achou; i++)
		for (auto j : adj[i]) {
			if (i == a and j.first == b) {
				achou = true;
				break;
			}
			posicao++;
		}
	posPeso.insert(posPeso.begin() + posicao, 0.5);
	isParal.insert(isParal.begin() + posicao, false);
	for (auto i : adj[b])
		if (i.first == a) {
			isParal[posicao] = true;

			int posicao2 = 0;
			for (int j = 0; j < G.getN(); j++)
				for (auto k : adj[j]) {
					if (j == b and k.first == a) isParal[posicao2] = true;
					posicao2++;
				}
		}
	this->resetColors();
}

// O(n+m)
void GraphDisplay::removeEdge(int e) {
	Graph G2(G.getN(), G.label);
	int posicao = 0;
	int a, b;
	auto adj = G.getAdj();
	for (int i = 0; i < G.getN(); i++)
		for (auto j : adj[i]) {
			if (posicao != e)
				G2.addEdge(i, j.first, j.second);
			else
				a = i, b = j.first;
			posicao++;
		}

	posicao = 0;
	for (int i = 0; i < G.getN(); i++)
		for (auto j : adj[i]) {
			if (i == b and j.first == a) isParal[posicao] = false;
			posicao++;
		}

	posPeso.erase(posPeso.begin() + e);
	isParal.erase(isParal.begin() + e);

	G = G2;
	this->resetColors();
}

// O(n^2 + m^2)
void GraphDisplay::itera() {
	if (draw) return;

	if (centr)
		fdpEadesAcc(2);
	else
		fdpEades(2);

	fdpPeso(2);
}

void GraphDisplay::getTikz(string arq, double scale) {
	ofstream outFile(arq);

	for (int i = 0; i < G.getN(); i++)
		outFile << "\\Vertex[x=" << scale * 5 * pos[i].x / X
				<< ",y=" << scale * 5 * pos[i].y / X << "]{" << G.label[i]
				<< "}\n";

	outFile << "\n";
	auto M = G.getMatrix();
	for (int i = 0; i < G.getN(); i++)
		for (int j = i + 1; j < G.getN(); j++)
			if (M[i][j] or M[j][i])
				outFile << "\\Edge(" << G.label[i] << ")(" << G.label[j]
						<< ")\n";

	outFile.close();
}
