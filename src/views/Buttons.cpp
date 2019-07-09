#include "Buttons.hpp"

namespace functions {
void coloreChordal(GraphCanvas *GC) {
	Chordal C(GC->GD.G);
	auto coloracao = C.coloring();
	GC->GD.color = coloracao;
}

void mst(GraphCanvas *GC) {
	auto v = GC->GD.G.mstEdges();
	for (int i = 0; i < GC->GD.G.getM(); i++)
		GC->GD.colorAresta[i] = (v[i] ? 1 : 100);
}

void greedyColoring(GraphCanvas *GC) {
	auto coloracao = GC->GD.G.greedyColoring();
	GC->GD.color = coloracao;
}

void artPoints(GraphCanvas *GC) {
	auto art = GC->GD.G.artPoints();
	for (int i = 0; i < GC->GD.G.getN(); i++) GC->GD.color[i] = art[i];
}

void pontes(GraphCanvas *GC) {
	auto pont = GC->GD.G.bridges();
	for (int i = 0; i < GC->GD.G.getM(); i++) GC->GD.colorAresta[i] = 100;
	map<pair<int, int>, int> id;
	auto adj = GC->GD.G.getAdj();
	int count = 0;
	for (int i = 0; i < GC->GD.G.getN(); i++)
		for (auto j : adj[i]) id[make_pair(i, j.first)] = count++;
	for (auto i : pont) {
		if (id.count(i)) GC->GD.colorAresta[id[i]] = 1;
		pair<int, int> j = make_pair(i.second, i.first);
		if (id.count(j)) GC->GD.colorAresta[id[j]] = 1;
	}
}

void center(GraphCanvas *GC) {
	for (int i = 0; i < GC->GD.G.getN(); i++) GC->GD.color[i] = 0;
	Tree T(GC->GD.G);
	GC->GD.color[T.center()] = 1;
}

void matching(GraphCanvas *GC) {
	auto match = GC->GD.G.blossom();
	for (int i = 0; i < GC->GD.G.getM(); i++) GC->GD.colorAresta[i] = 100;
	map<pair<int, int>, int> id;
	auto adj = GC->GD.G.getAdj();
	int count = 0;
	for (int i = 0; i < GC->GD.G.getN(); i++)
		for (auto j : adj[i]) id[make_pair(i, j.first)] = count++;
	for (int i = 0; i < GC->GD.G.getN(); i++)
		if (match[i] != -1 and match[i] > i) {
			pair<int, int> j = make_pair(i, match[i]),
						   jj = make_pair(match[i], i);
			if (id.count(j)) GC->GD.colorAresta[id[j]] = 1;
			if (id.count(jj)) GC->GD.colorAresta[id[jj]] = 1;
		}
}

void clearGraph(GraphCanvas *GC) {
	GC->GD.color = vector<int>(GC->GD.G.getN(), 0);
	GC->GD.colorAresta = vector<int>(GC->GD.G.getM(), 100);
}

} // namespace functions

namespace buttons {
void general(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {
	vector<int> op = {1, 2, 3, 4, 6, 7};
	for (int i : op) gui.add(v[i]);
}

void clear(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {
	for (auto i : v) gui.remove(i);
}

void bipartite(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {}

void chordal(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {
	vector<int> op = {0, 6};
	for (int i : op) gui.add(v[i]);
}

void dag(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {}

void tree(tgui::Gui &gui, vector<tgui::Button::Ptr> &v) {
	vector<int> op = {5};
	for (int i : op) gui.add(v[i]);
}

void init(vector<tgui::Button::Ptr> &v, GraphCanvas &GC) {
	auto color = tgui::Button::create("Colore");
	v.push_back(color);
	color->setSize(75.f, 20.f);
	color->setPosition(810.f, 260.f);
	color->connect("pressed", functions::coloreChordal, &GC);

	auto mst = tgui::Button::create("mst");
	v.push_back(mst);
	mst->setSize(75.f, 20.f);
	mst->setPosition(810.f, 70.f);
	mst->connect("pressed", functions::mst, &GC);

	auto guloso = tgui::Button::create("Colore (guloso)");
	v.push_back(guloso);
	guloso->setSize(115.f, 20.f);
	guloso->setPosition(810.f, 210.f);
	guloso->connect("pressed", functions::greedyColoring, &GC);

	auto artPoints = tgui::Button::create("Pontos de art");
	v.push_back(artPoints);
	artPoints->setSize(115.f, 20.f);
	artPoints->setPosition(810.f, 110.f);
	artPoints->connect("pressed", functions::artPoints, &GC);

	auto pontes = tgui::Button::create("Pontes");
	v.push_back(pontes);
	pontes->setSize(75.f, 20.f);
	pontes->setPosition(810.f, 160.f);
	pontes->connect("pressed", functions::pontes, &GC);

	auto center = tgui::Button::create("Centro");
	v.push_back(center);
	center->setSize(75.f, 20.f);
	center->setPosition(810.f, 310.f);
	center->connect("pressed", functions::center, &GC);

	auto clear = tgui::Button::create("Limpa");
	v.push_back(clear);
	clear->setSize(75.f, 20.f);
	clear->setPosition(810.f, 360.f);
	clear->connect("pressed", functions::clearGraph, &GC);

	auto matching = tgui::Button::create("matching");
	v.push_back(matching);
	matching->setSize(75.f, 20.f);
	matching->setPosition(810.f, 30.f);
	matching->connect("pressed", functions::matching, &GC);
}

void update(tgui::Gui &gui, vector<tgui::Button::Ptr> &botoes,
			GraphCanvas &GC) {
	for (int i = 0; i < 10; i++) clear(gui, botoes);
	if (GC.GD.G.getN()) {
		general(gui, botoes);
		if (GC.GD.G.isBipartite()) bipartite(gui, botoes);
		if (GC.GD.G.isChordal()) chordal(gui, botoes);
		if (GC.GD.G.isDag()) dag(gui, botoes);
		if (GC.GD.G.isTree()) tree(gui, botoes);
	}
}
} // namespace buttons
