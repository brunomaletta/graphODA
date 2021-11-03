#include "Repl.hpp"

namespace repl {

vector<TYPE> graphs;
map<string, int> vars;
stringstream ss;
string var;
int at, lastVar;

// aux

string getType(int i) {
	if (i == 0) return "Graph";
	if (i == 1) return "Bipartite";
	if (i == 2) return "Chordal";
	if (i == 3) return "Dag";
	if (i == 4) return "Tree";
	return "";
}

Graph getGraphVar(string var2) {
	Graph G;
	int att = vars[var2];
	if (graphs[att].type == 0) G = Graph(graphs[att].G);
	if (graphs[att].type == 1) G = Graph(graphs[att].B);
	if (graphs[att].type == 2) G = Graph(graphs[att].C);
	if (graphs[att].type == 3) G = Graph(graphs[att].D);
	if (graphs[att].type == 4) G = Graph(graphs[att].T);
	return G;
}

Graph getGraph() { return getGraphVar(var); }

int getVertex(Graph &G, string s) {
	for (int i = 0; i < G.getN(); i++)
		if (G.label[i] == s) return i;
	throw REPLVertexNotFoundException(s);
}

void undoDeclaration() {
	if (lastVar == -1) {
		vars.erase(var);
		graphs.pop_back();
	}
}

// errors

void fileNotFoundError(string f) {
	cout << "ERROR: file '" << f << "' not found" << endl;
}

void undefinedVariableError(string var) {
	cout << "ERROR: undefined variable '" << var << "'" << endl;
}

void wrongTypeError(string var, int exp) {
	cout << "ERROR: " << var << " has type '" << getType(graphs[vars[var]].type)
		 << "'" << endl;
	cout << "\texpected '" << getType(exp) << "'" << endl;
}

void graphIsNotError(string var, int exp) {
	string expS = getType(exp);
	expS[0] += 'a' - 'A';
	cout << "ERROR: " << var << " is not " << expS << endl;
}

void vertexNotFoundError(string v) {
	cout << "ERROR: vertex '" << v << "' not found" << endl;
}

void noPathError(string a, string b) {
	cout << "ERROR: no path from '" << a << "' to '" << b << "'" << endl;
}

void negativeCycleError() {
	cout << "ERROR: graph has a negative cycle" << endl;
}

// commands

void import() {
	string file;
	if (!(ss >> file)) throw REPLInvalidCommandException();
	Graph j;
	try {
		j = lerGrafoArquivo(file);
	} catch (FileNotFoundException &e) {
		fileNotFoundError(file);
		undoDeclaration();
		return;
	}
	if (j.getN()) {
		graphs[at].G = j;
		graphs[at].type = 0;
	}
}

void mst() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	if (graphs[vars[var2]].type != 0) {
		wrongTypeError(var2, 0);
		undoDeclaration();
		return;
	}
	graphs[at].T = graphs[vars[var2]].G.mst();
	graphs[at].type = 4;
}

void graphCast() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	Graph G = getGraphVar(var2);
	graphs[at].G = G;
	graphs[at].type = 0;
}

void bipartiteCast() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	Graph G = getGraphVar(var2);
	if (!G.isBipartite()) {
		graphIsNotError(var2, 1);
		undoDeclaration();
		return;
	}
	graphs[at].B = Bipartite(G);
	graphs[at].type = 1;
}

void chordalCast() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	Graph G = getGraphVar(var2);
	if (!G.isChordal()) {
		graphIsNotError(var2, 2);
		undoDeclaration();
		return;
	}
	graphs[at].C = Chordal(G);
	graphs[at].type = 2;
}

void dagCast() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	Graph G = getGraphVar(var2);
	if (!G.isDag()) {
		graphIsNotError(var2, 3);
		undoDeclaration();
		return;
	}
	graphs[at].D = Dag(G);
	graphs[at].type = 3;
}

void treeCast() {
	string var2;
	if (!(ss >> var2)) throw REPLInvalidCommandException();
	if (!vars.count(var2)) {
		undefinedVariableError(var2);
		undoDeclaration();
		return;
	}
	Graph G = getGraphVar(var2);
	if (!G.isTree()) {
		graphIsNotError(var2, 4);
		undoDeclaration();
		return;
	}
	graphs[at].T = Tree(G);
	graphs[at].type = 4;
}

void show(int X, int Y) { interface::display(X, Y, getGraph()); }

void edit(int X, int Y) {
	graphs[at].G = interface::display(X, Y, getGraph());
	graphs[at].type = 0;
}

void describe() {
	Graph G = getGraph();
	cout << "Type of " << var << ": " << getType(graphs[at].type) << endl;
	cout << G.getN() << " vertices and " << G.getM() << " edges" << endl;
	cout << endl;

	cout << "bipartite:   ";
	if (G.isBipartite())
		cout << "yes" << endl;
	else
		cout << " no" << endl;

	cout << "chordal:     ";
	if (G.isChordal())
		cout << "yes" << endl;
	else
		cout << " no" << endl;

	cout << "dag:         ";
	if (G.isDag())
		cout << "yes" << endl;
	else
		cout << " no" << endl;

	cout << "tree:        ";
	if (G.isTree())
		cout << "yes" << endl;
	else
		cout << " no" << endl;
}

void reaches() {
	Graph G = getGraph();
	string a, b;
	if (!(ss >> a >> b)) throw REPLInvalidCommandException();
	int aa, bb;
	try {
		aa = getVertex(G, a);
	} catch (REPLVertexNotFoundException &e) {
		vertexNotFoundError(a);
		return;
	}
	try {
		bb = getVertex(G, b);
	} catch (REPLVertexNotFoundException &e) {
		vertexNotFoundError(b);
		return;
	}

	if (G.reaches(aa, bb))
		cout << "true" << endl;
	else
		cout << "false" << endl;
}

// O(n+m)
void scc() {
	Graph G = getGraph();
	auto v = G.scc();
	int ma = 0;
	for (int i : v) ma = max(ma, i);
	cout << ma + 1 << " strongly connected componentes" << endl << endl;

	vector<vector<int> > v2(ma + 1);
	for (int i = 0; i < G.getN(); i++) v2[v[i]].push_back(i);
	for (int i = 0; i <= ma; i++) {
		cout << i + 1 << ":";
		for (int j : v2[i]) cout << " " << G.label[j];
		cout << endl;
	}
}

void shortestPath() {
	string a, b;
	if (!(ss >> a >> b)) throw REPLInvalidCommandException();
	Graph G = getGraph();
	int aa, bb;
	try {
		aa = getVertex(G, a);
	} catch (REPLVertexNotFoundException &e) {
		vertexNotFoundError(a);
		return;
	}
	try {
		bb = getVertex(G, b);
	} catch (REPLVertexNotFoundException &e) {
		vertexNotFoundError(b);
		return;
	}
	try {
		cout << G.shortestPath(aa, bb) << endl;
	} catch (GraphNoPathException &e) {
		noPathError(a, b);
	} catch (GraphNegativeCycleException &e) {
		negativeCycleError();
	}
}

void coloring() {
	try {
		Graph H = getGraph();
		Chordal G(H);

		cout << "Coloring" << endl << endl;
		vector<int> cores = G.coloring();
		for (int i = 0; i < G.getN(); i++)
			cout << G.label[i] << ": " << cores[i] << endl;

	} catch (ChordalConstructorException &e) {
		wrongTypeError(var, 2);
	}
}

void chromaticNumber() {
	try {
		Graph H = getGraph();
		Chordal G(H);
		cout << "Chromatic number: " << G.chromaticNumber() << endl;
	} catch (ChordalConstructorException &e) {
		graphIsNotError(var, 2);
	}
}

void greedyColoring() {
	Graph G = getGraph();
	cout << "Coloring" << endl << endl;
	vector<int> cores = G.greedyColoring();
	for (int i = 0; i < G.getN(); i++)
		cout << G.label[i] << ": " << cores[i] << endl;
}

void maxClique() {
	try {
		Graph H = getGraph();
		Chordal G(H);
		cout << "Maximum clique size: " << G.maxClique() << endl;
	} catch (ChordalConstructorException &e) {
		graphIsNotError(var, 2);
	}
}

void artPoints() {
	Graph G = getGraph();
	auto art = G.artPoints();
	cout << "Articulation points:" << endl;
	for (int i = 0; i < G.getN(); i++)
		if (art[i]) cout << G.label[i] << " ";
	cout << endl;
}

void bridges() {
	Graph G = getGraph();
	auto bridge = G.bridges();
	cout << "Bridges: " << endl;
	for (auto i : bridge)
		cout << "(" << G.label[i.first] << ", " << G.label[i.second] << ")"
			 << endl;
}

void topoSort() {
	try {
		Graph H = getGraph();
		Dag G(H);
		vector<int> topo = G.topoSort();
		cout << "Topological ordering:" << endl;
		for (int i : topo) cout << G.label[i] << " ";
		cout << endl;
	} catch (DagConstructorException &e) {
		graphIsNotError(var, 3);
	}
}

void center() {
	try {
		Graph H = getGraph();
		Tree G(H);
		int c = G.center();
		cout << G.label[c] << endl;
	} catch (TreeConstructorException &e) {
		graphIsNotError(var, 4);
	}
}

void diameter() {
	try {
		Graph H = getGraph();
		Tree G(H);
		int diam = G.diameter();
		cout << diam << endl;
	} catch (TreeConstructorException &e) {
		graphIsNotError(var, 4);
	}
}

void matching() {
	Graph G = getGraph();
	auto match = G.blossom();
	int ans = 0;
	for (auto i : match)
		if (i > -1) ans++;
	cout << "Maximum matching has cardinality " << ans / 2 << endl;
	for (int i = 0; i < G.getN(); i++)
		if (match[i] != -1 and match[i] > i)
			cout << G.label[i] << " " << G.label[match[i]] << endl;
}

void tikz(int X, int Y) {
	string file;
	ss >> file;
	if (file == "") throw REPLInvalidCommandException();
	Graph G = getGraph();
	double scale;
	if (!(ss >> scale)) scale = 1;
	try {
		GraphDisplay GD(G, X * 4 / 5, Y * 6 / 7);
		GD.good(max(10, 100 - GD.G.getN()), max(10, 100 - GD.G.getM()));
		GD.getTikz(file, scale);
	} catch (FileNotFoundException &e) {
		fileNotFoundError(file);
	}
}

void run(int X, int Y) {
	cout << "GraphODA" << endl;

	while (1) {
		cout << ">>> ";

		// le a linha
		string l;
		getline(cin, l);
		// acabou
		if (!cin) {
			cout << endl;
			break;
		}

		ss = stringstream(l);
		var.clear();
		char c;
		int atribuicao = -1;
		while (ss >> c) {
			if (c == '=') {
				atribuicao = 1;
				break;
			}
			if (c == '>') {
				atribuicao = 0;
				break;
			}
			var += c;
		}
		if (atribuicao == -1) {
			cout << "ERROR: what are you doing?" << endl;
			continue;
		}

		// coloca no map
		if (!vars.count(var)) {
			if (!atribuicao) {
				undefinedVariableError(var);
				continue;
			}
			lastVar = -1;

			vars[var] = graphs.size();
			TYPE new_;
			new_.G = Graph();
			new_.type = 0;
			graphs.push_back(new_);
		} else
			lastVar = vars[var];

		at = vars[var];
		try {
			if (atribuicao) {
				string com;
				ss >> com;

				if (com == "import")
					import();
				else if (com == "mst")
					mst();
				else if (com == "(bipartite)")
					bipartiteCast();
				else if (com == "(chordal)")
					chordalCast();
				else if (com == "(dag)")
					dagCast();
				else if (com == "(graph)")
					graphCast();
				else if (com == "(tree)")
					treeCast();
				else {
					// a = b
					if (!vars.count(com)) {
						undefinedVariableError(com);
						undoDeclaration();
						continue;
					}
					if (var == com) {
						undoDeclaration();
						continue;
					}

					graphs[at].type = graphs[vars[com]].type;
					if (graphs[at].type == 0)
						graphs[at].G = graphs[vars[com]].G;
					if (graphs[at].type == 1)
						graphs[at].B = graphs[vars[com]].B;
					if (graphs[at].type == 2)
						graphs[at].C = graphs[vars[com]].C;
					if (graphs[at].type == 3)
						graphs[at].D = graphs[vars[com]].D;
					if (graphs[at].type == 4)
						graphs[at].T = graphs[vars[com]].T;
				}
			} else {
				string com;
				ss >> com;

				if (com == "show")
					show(X, Y);
				else if (com == "edit")
					edit(X, Y);
				else if (com == "describe")
					describe();
				else if (com == "reaches")
					reaches();
				else if (com == "scc")
					scc();
				else if (com == "shortestPath")
					shortestPath();
				else if (com == "coloring")
					coloring();
				else if (com == "chromaticNumber")
					chromaticNumber();
				else if (com == "greedyColoring")
					greedyColoring();
				else if (com == "maximumCliqueSize")
					maxClique();
				else if (com == "artPoints")
					artPoints();
				else if (com == "bridges")
					bridges();
				else if (com == "topoSort")
					topoSort();
				else if (com == "center")
					center();
				else if (com == "diameter")
					diameter();
				else if (com == "matching")
					matching();
				else if (com == "tikz")
					tikz(X, Y);
				else
					throw REPLInvalidCommandException();
			}
		} catch (REPLInvalidCommandException &e) {
			cout << "ERROR: invalid command" << endl;
			if (atribuicao) undoDeclaration();
		}
	}
}
} // namespace repl
