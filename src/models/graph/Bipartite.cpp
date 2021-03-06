#include "Bipartite.hpp"

Bipartite::Bipartite() : GraphGen() {}

Bipartite::Bipartite(int n) : GraphGen(n) {}

Bipartite::Bipartite(GraphGen& G) {
	if (!G.isBipartite()) throw BipartiteConstructorException();

	n = G.getN();
	m = G.getM();
	label = G.label;
	adj = G.getAdj();
	simAdj = G.getSimAdj();
}
