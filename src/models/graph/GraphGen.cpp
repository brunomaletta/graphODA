#include "GraphGen.hpp"

GraphGen::GraphGen() { n = 0, m = 0; }

GraphGen::GraphGen(int n_) {
	n = n_;
	m = 0;
	adj.resize(n);
	simAdj.resize(n);
	for (int i = 0; i < n; i++) label.push_back(to_string(i));
}

GraphGen::GraphGen(int n_, vector<string>& label_) {
	n = n_;
	m = 0;
	adj.resize(n);
	simAdj.resize(n);
	for (int i = 0; i < n; i++) {
		if (i >= label_.size())
			label.push_back(to_string(i));
		else
			label.push_back(label_[i]);
	}
}

int GraphGen::getN() { return n; }

int GraphGen::getM() { return m; }

// O(n+m)
vector<vector<pair<int, int>>> GraphGen::getAdj() { return adj; }

vector<vector<pair<int, int>>> GraphGen::getSimAdj() { return simAdj; }

// O(n+m)
vector<pair<int, int>> GraphGen::getEdges() {
	vector<pair<int, int>> edg;
	for (int i = 0; i < n; i++)
		for (auto j : adj[i]) edg.push_back({i, j.first});
	return edg;
}

vector<int> GraphGen::getPesos() {
	vector<int> pes;
	for (int i = 0; i < n; i++)
		for (auto j : adj[i]) pes.push_back(j.second);
	return pes;
}

// O(n^2)
vector<vector<int>> GraphGen::getMatrix() {
	vector<vector<int>> matrix(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++)
		for (auto j : adj[i]) matrix[i][j.first] = 1;
	return matrix;
}

// O(n^2)
vector<vector<int>> GraphGen::getSimMatrix() {
	vector<vector<int>> matrix(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++)
		for (auto j : adj[i]) matrix[i][j.first] = matrix[j.first][i] = 1;
	return matrix;
}

// O(n+m)
bool GraphGen::isWeighted() {
	for (int i = 0; i < n; i++)
		for (auto j : adj[i])
			if (j.second != 1) return true;
	return false;
}

// O(n+m)
bool GraphGen::hasNegativeWeight() {
	for (int i = 0; i < n; i++)
		for (auto j : adj[i])
			if (j.second < 0) return true;
	return false;
}

bool GraphGen::dfsReaches(vector<bool>& vis, int a, int b) {
	vis[a] = true;
	if (a == b) return true;

	for (auto i : adj[a])
		if (!vis[i.first] and dfsReaches(vis, i.first, b)) return true;
	return false;
}

bool GraphGen::reaches(int a, int b) {
	vector<bool> vis(n, false);
	return dfsReaches(vis, a, b);
}

void GraphGen::dfsCheckConnected(vector<bool>& vis, int v) {
	vis[v] = true;
	for (auto i : simAdj[v])
		if (!vis[i.first]) dfsCheckConnected(vis, i.first);
}

bool GraphGen::isConnected() {
	vector<bool> vis(n, false);
	if (n) dfsCheckConnected(vis, 0);
	for (int i = 0; i < n; i++)
		if (!vis[i]) return false;
	return true;
}

bool GraphGen::dfsCheckBipartite(int i, int c, vector<int>& cor) {
	cor[i] = c;
	for (auto j : simAdj[i]) {
		if (cor[j.first] == c) return false;
		if (cor[j.first] == -1 and !dfsCheckBipartite(j.first, !c, cor))
			return false;
	}
	return true;
}

// O(n+m)
bool GraphGen::isBipartite() {
	vector<int> cor(n, -1);
	for (int i = 0; i < n; i++)
		if (cor[i] == -1 and !dfsCheckBipartite(i, 0, cor)) return false;
	return true;
}

pair<vector<int>, vector<int>> GraphGen::maximumCardinalitySearch() {
	vector<set<int>> Set(n);
	vector<int> size(n, 0);
	vector<int> alpha(n);
	vector<int> alphaInv(n);
	vector<bool> visitado(n, 0);

	for (int k = 0; k < n; k++) Set[0].insert(k);

	// Maximum Cardinality Search
	for (int i = n - 1, j = 0; i >= 0; i--) {
		int v = *Set[j].begin();
		Set[j].erase(v);
		j++;

		alpha[v] = i;
		alphaInv[i] = v;
		size[v] = -1;

		fill(visitado.begin(), visitado.end(), false);
		for (auto u : simAdj[v]) {
			int w = u.first;
			if (visitado[w] || size[w] < 0) continue;

			visitado[w] = true;
			Set[size[w]].erase(w);
			size[w]++;
			Set[size[w]].insert(w);
		}

		while (j > 0 && Set[j].empty()) j--;
	}

	return make_pair(alpha, alphaInv);
}

bool GraphGen::checkOrdering(vector<int> ordering) {
	if (ordering.size() != n) return false;

	vector<bool> checker(n, false);
	int count = 0;

	for (auto x : ordering) {
		if (x >= n || x < 0) return false;

		if (!checker[x]) {
			checker[x] = true;
			count++;
		}
	}

	return count == n;
}

bool GraphGen::zeroFillIn(pair<vector<int>, vector<int>> parAlpha) {
	vector<int> alpha = parAlpha.first;
	vector<int> alphaInv = parAlpha.second;

	if (!checkOrdering(alpha)) throw GraphOrderingException();
	if (!checkOrdering(alphaInv)) throw GraphOrderingException();

	vector<int> f(n);
	vector<int> index(n);
	vector<bool> visitado(n, 0);

	// Zero Fill-in
	for (int i = 0; i < n; i++) {
		int w = alphaInv[i];
		f[w] = w;
		index[w] = i;

		fill(visitado.begin(), visitado.end(), false);
		for (auto u : simAdj[w]) {
			int v = u.first;
			if (visitado[v]) continue;

			if (alpha[v] >= i) continue;

			visitado[v] = true;
			index[v] = i;

			if (f[v] == v) f[v] = w;
		}

		fill(visitado.begin(), visitado.end(), false);
		for (auto u : simAdj[w]) {
			int v = u.first;
			if (visitado[v]) continue;

			if (alpha[v] >= i) continue;

			visitado[v] = true;
			if (index[f[v]] < i) return false;
		}
	}
	return true;
}

bool GraphGen::isChordal() { return zeroFillIn(maximumCardinalitySearch()); }

bool GraphGen::dfsCheckDag(int i, vector<int>& vis) {
	vis[i] = 1;
	for (auto j : adj[i]) {
		if (vis[j.first] == 1) return false;
		if (!vis[j.first] and !dfsCheckDag(j.first, vis)) return false;
	}
	vis[i] = 2;
	return true;
}

// O(n+m)
bool GraphGen::isDag() {
	vector<int> vis(n, 0);
	for (int i = 0; i < n; i++)
		if (!vis[i] and !dfsCheckDag(i, vis)) return false;
	return true;
}

bool GraphGen::dfsCheckTree(int i, int last, vector<int>& vis) {
	vis[i] = 1;
	for (auto j : simAdj[i]) {
		if (j.first != last and vis[j.first] == 1) return false;
		if (!vis[j.first] and !dfsCheckTree(j.first, i, vis)) return false;
	}
	vis[i] = 2;
	return true;
}

// O(n+m)
bool GraphGen::isTree() {
	if (!isConnected()) return false;
	vector<int> vis(n, 0);
	for (int i = 0; i < n; i++)
		if (!vis[i] and !dfsCheckTree(i, i, vis)) return false;
	return true;
}

// algoritmo de Tarjan para SCC
int GraphGen::dfsScc(vector<int>& comp, vector<int>& vis, vector<int>& id,
					 int& t, int& c, int k) {
	static stack<int> s;

	int lo = id[k] = t++;
	s.push(k);
	vis[k] = 2;

	for (int i = 0; i < adj[k].size(); i++) {
		if (!vis[adj[k][i].first])
			lo = min(lo, dfsScc(comp, vis, id, t, c, adj[k][i].first));
		else if (vis[adj[k][i].first] == 2)
			lo = min(lo, id[adj[k][i].first]);
	}
	if (lo == id[k])
		while (1) {
			int u = s.top();
			s.pop();
			vis[u] = 1;
			comp[u] = c;
			if (u == k) {
				c++;
				break;
			}
		}
	return lo;
}

// O(n+m)
vector<int> GraphGen::scc() {
	vector<int> vis(n, 0), comp(n), id(n);
	int t = -1, c = 0;
	for (int i = 0; i < n; i++)
		if (!vis[i]) dfsScc(comp, vis, id, ++t, c, i);
	return comp;
}

// O(n+m)
long long GraphGen::shortestPathBFS(int a, int b) {
	vector<bool> vis(n, 0);
	queue<pair<int, int>> q;
	vis[a] = true;
	q.push({a, 0});

	while (q.size()) {
		int u = q.front().first, d = q.front().second;
		q.pop();

		if (u == b) return d;
		for (auto i : adj[u])
			if (!vis[i.first]) {
				q.push({i.first, d + 1});
				vis[i.first] = true;
			}
	}
	throw GraphNoPathException(a, b);
}

// O(m log(n))
long long GraphGen::dijkstra(int a, int b) {
	vector<long long> d(n, 0x3f3f3f3f3f3f3f3fll);
	priority_queue<pair<long long, int>, vector<pair<long long, int>>,
				   greater<pair<long long, int>>>
		q;
	d[a] = 0;
	q.push({0, a});

	while (q.size()) {
		int u = q.top().second;
		long long dist = q.top().first;
		q.pop();
		if (dist > d[u]) continue;

		for (auto i : adj[u]) {
			int v = i.first, w = i.second;
			if (d[v] > d[u] + w) {
				d[v] = d[u] + w;
				q.push({d[v], v});
			}
		}
	}
	return d[b];
}

long long GraphGen::bellmanFord(int a, int b) {
	vector<long long> d(n, 0x3f3f3f3f3f3f3f3fll);
	d[a] = 0;
	auto edg = this->getEdges();
	auto pesos = this->getPesos();

	for (int i = 0; i <= n; i++)
		for (int j = 0; j < m; j++) {
			if (d[edg[j].first] + pesos[j] < d[edg[j].second]) {
				if (i == n) throw GraphNegativeCycleException();

				d[edg[j].second] = d[edg[j].first] + pesos[j];
			}
		}

	return d[b];
}

long long GraphGen::shortestPath(int a, int b) {
	if (!this->reaches(a, b)) throw GraphNoPathException(a, b);

	if (this->hasNegativeWeight()) return bellmanFord(a, b);
	if (!this->isWeighted()) return shortestPathBFS(a, b);
	return dijkstra(a, b);
}

// Recebe uma ordem dos vértices e colore
vector<int> GraphGen::greedyColoring(vector<int> alpha) {
	if (!checkOrdering(alpha)) throw GraphOrderingException();

	vector<int> colors(n, 0);
	vector<int> vizinhanca;
	vector<bool> visitado(n, false);

	// Itera pela ordenação de trás para frente
	for (int i = n - 1; i >= 0; i--) {
		int max = 0;
		vizinhanca.clear();
		fill(visitado.begin(), visitado.end(), false);

		// Vê quais cores dos vizinhos
		for (auto u : simAdj[alpha[i]]) {
			int w = u.first;
			if (visitado[w]) continue;

			visitado[w] = true;
			if (colors[w]) vizinhanca.push_back(colors[w]);
			max++;
		}

		// Marca as cores usadas
		vector<int> possiveis(max + 1, 0);
		for (auto c : vizinhanca)
			if (c <= max) possiveis[c - 1]++;

		// Seleciona a menor cor não usada
		for (int j = 0; j <= max; j++)
			if (possiveis[j] == 0) {
				colors[alpha[i]] = j + 1;
				break;
			}

		if (!max) colors[alpha[i]] = 1;
	}

	return colors;
}

vector<int> GraphGen::greedyColoring() {
	vector<int> ordem(n);
	iota(ordem.begin(), ordem.end(), 0);
	return greedyColoring(ordem);
}

void GraphGen::dfsArt(vector<bool>& vis, vector<int>& in, vector<int>& low,
					  vector<int>& parent, vector<bool>& isArt,
					  vector<pair<int, int>>& bridge, int v, int p, int& d) {
	parent[v] = p;
	low[v] = in[v] = d++;
	isArt[v] = false;
	for (auto j : simAdj[v]) {
		if (j.first == p) continue;
		if (in[j.first] == -1) {
			dfsArt(vis, in, low, parent, isArt, bridge, j.first, v, d);
			if (low[j.first] >= in[v]) isArt[v] = true;
			if (low[j.first] > in[v]) bridge.push_back({v, j.first});
			low[v] = min(low[v], low[j.first]);
		} else
			low[v] = min(low[v], in[j.first]);
	}
	if (p == -1) {
		isArt[v] = false;
		int k = 0;
		for (auto j : simAdj[v])
			if (!vis[j.first]) {
				vis[j.first] = true;
				k += (parent[j.first] == v);
			}
		if (k > 1) isArt[v] = true;
	}
}

vector<bool> GraphGen::artPoints() {
	vector<bool> isArt(n), vis(n, false);
	vector<int> in(n, -1), low(n), parent(n);
	vector<pair<int, int>> bridge;
	int d = -1;
	for (int i = 0; i < n; i++)
		if (in[i] == -1)
			dfsArt(vis, in, low, parent, isArt, bridge, i, -1, ++d);
	return isArt;
}

vector<pair<int, int>> GraphGen::bridges() {
	vector<bool> isArt(n), vis(n, false);
	vector<int> in(n, -1), low(n), parent(n);
	vector<pair<int, int>> bridge;
	int d = -1;
	for (int i = 0; i < n; i++)
		if (in[i] == -1)
			dfsArt(vis, in, low, parent, isArt, bridge, i, -1, ++d);
	return bridge;
}

// TODO
vector<int> GraphGen::coloring() { return vector<int>(); }

void GraphGen::contractBlossom(vector<int>& match, vector<int>& pai,
							   vector<int>& base, vector<int>& vis,
							   queue<int>& q, int u, int v, bool first) {
	static vector<bool> bloss;
	static int l;
	if (first) {
		bloss = vector<bool>(n, 0);
		vector<bool> teve(n, 0);
		int k = u;
		l = v;
		while (1) {
			teve[k = base[k]] = 1;
			if (match[k] == -1) break;
			k = pai[match[k]];
		}
		while (!teve[l = base[l]]) l = pai[match[l]];
	}
	while (base[u] != l) {
		bloss[base[u]] = bloss[base[match[u]]] = 1;
		pai[u] = v;
		v = match[u];
		u = pai[match[u]];
	}
	if (!first) return;
	contractBlossom(match, pai, base, vis, q, v, u, 0);
	for (int i = 0; i < n; i++)
		if (bloss[base[i]]) {
			base[i] = l;
			if (!vis[i]) q.push(i);
			vis[i] = 1;
		}
}

int GraphGen::getAugmentingPath(vector<int>& match, vector<int>& pai,
								vector<int>& base, vector<int>& vis,
								queue<int>& q, int s) {
	for (int i = 0; i < n; i++) base[i] = i, pai[i] = -1, vis[i] = 0;
	vis[s] = 1;
	q = queue<int>();
	q.push(s);
	while (q.size()) {
		int u = q.front();
		q.pop();
		for (auto j : simAdj[u]) {
			int i = j.first;
			if (base[i] == base[u] or match[u] == i) continue;
			if (i == s or (match[i] != -1 and pai[match[i]] != -1))
				contractBlossom(match, pai, base, vis, q, u, i);
			else if (pai[i] == -1) {
				pai[i] = u;
				if (match[i] == -1) return i;
				i = match[i];
				vis[i] = 1;
				q.push(i);
			}
		}
	}
	return -1;
}

vector<int> GraphGen::blossom() {
	vector<int> match(n, -1), pai(n), base(n), vis(n);
	queue<int> q;
	for (int i = 0; i < n; i++)
		if (match[i] == -1)
			for (auto j : simAdj[i])
				if (match[j.first] == -1) {
					match[i] = j.first;
					match[j.first] = i;
					break;
				}
	for (int i = 0; i < n; i++)
		if (match[i] == -1) {
			int j = getAugmentingPath(match, pai, base, vis, q, i);
			if (j == -1) continue;
			while (j != -1) {
				int p = pai[j], pp = match[p];
				match[p] = j;
				match[j] = p;
				j = pp;
			}
		}
	return match;
}

int GraphGen::cmpDouble(double x, double y, double EPS) {
	double dist = abs(x-y);
	if (dist < EPS) return 0;
	return (x < y) ? -1 : 1;
}

vector<int> GraphGen::inertia(vector<vector<double>> a) {
	int n = a.size();
	for (int k = 0; k < n; k++) {
		if (!cmpDouble(a[k][k], 0)) {
			for (int i = k; i < n; i++) {
				if (cmpDouble(a[i][k], 0)) {
					vector<double> vec = a[i];

					for (int j = 0; j < n; j++) {
						a[k][j] += vec[j];
						a[j][k] += vec[j];
					}
					break;
				}
			}
		}
		if (!cmpDouble(a[k][k], 0)) continue;
		double alpha = sqrt(abs(a[k][k]));
		for (int i = k; i < n; i++) {
			a[i][k] /= alpha;
			a[k][i] /= alpha;
		}

		for (int i = k+1; i < n; i++) {
			double beta = a[i][k]/a[k][k];
			for (int j = 0; j < n; j++) {
				a[i][j] -= beta*a[k][j];
			}
			double gamma = a[k][i]/a[k][k];
			for (int j = 0; j < n; j++) {
				a[j][i] -= gamma*a[j][k];
			}
		}
	}
	vector<int> ans(3);
	for (int k = 0; k < n; k++) {
		ans[cmpDouble(a[k][k], 0, 1e-9) + 1]++;
	}
	return ans;
}

void GraphGen::eigenvalueRec(const vector<vector<int>>& t, vector<double>& eig,
			double l, double r, int sz_l, int sz_r) {
	if (sz_l + sz_r == t.size()) return;

	double m = (l+r)/2;

	if (!cmpDouble(l, r, 1e-6)) {
		for (int k = 0; k < t.size()-sz_l-sz_r; k++)
			eig.push_back(m);
		return;
	}

	vector<vector<double>> a(t.size(), vector<double>(t.size()));
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a.size(); j++) {
			a[i][j] = t[i][j];
			if (i == j) a[i][j] -= m;
		}
	}

	auto i = inertia(a);

	eigenvalueRec(t, eig, l, m, sz_l, i[1] + i[2]);

	for (int k = 0; k < i[1]; k++) eig.push_back(m);

	eigenvalueRec(t, eig, m, r, i[0] + i[1], sz_r);
}

vector<double> GraphGen::getEigenvalues() {
	vector<double> ret;
	eigenvalueRec(getSimMatrix(), ret, -getN(), getN(), 0, 0);
	return ret;
}
