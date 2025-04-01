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

void add_buttons(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v,
	const vector<int> &idxs) {
	for (int idx : idxs) if (!gui.get(v[idx].second))
		gui.add(v[idx].first, v[idx].second);
}

} // namespace functions

namespace buttons {
void general(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {
	functions::add_buttons(gui, v, {1, 2, 3, 4, 6, 7});
}

void clear(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {
	for (auto& [button, name] : v) gui.remove(button);
}

void bipartite(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {}

void chordal(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {
	functions::add_buttons(gui, v, {0, 6});
}

void dag(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {}

void tree(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &v) {
	functions::add_buttons(gui, v, {5});
}

void init(vector<pair<tgui::Button::Ptr, string>> &v, GraphCanvas &GC) {
	auto color = tgui::Button::create("Colore");
	v.emplace_back(color, "Colore");
	color->setSize(75.f, 20.f);
	color->setPosition(810.f, 260.f);
	color->onPress(functions::coloreChordal, &GC);

	auto mst = tgui::Button::create("mst");
	v.emplace_back(mst, "mst");
	mst->setSize(75.f, 20.f);
	mst->setPosition(810.f, 70.f);
	mst->onPress(functions::mst, &GC);

	auto guloso = tgui::Button::create("Colore (guloso)");
	v.emplace_back(guloso, "Colore (guloso)");
	guloso->setSize(115.f, 20.f);
	guloso->setPosition(810.f, 210.f);
	guloso->onPress(functions::greedyColoring, &GC);

	auto artPoints = tgui::Button::create("Pontos de art");
	v.emplace_back(artPoints, "Pontos de art");
	artPoints->setSize(115.f, 20.f);
	artPoints->setPosition(810.f, 110.f);
	artPoints->onPress(functions::artPoints, &GC);

	auto pontes = tgui::Button::create("Pontes");
	v.emplace_back(pontes, "Pontes");
	pontes->setSize(75.f, 20.f);
	pontes->setPosition(810.f, 160.f);
	pontes->onPress(functions::pontes, &GC);

	auto center = tgui::Button::create("Centro");
	v.emplace_back(center, "Centro");
	center->setSize(75.f, 20.f);
	center->setPosition(810.f, 310.f);
	center->onPress(functions::center, &GC);

	auto clear = tgui::Button::create("Limpa");
	v.emplace_back(clear, "Limpa");
	clear->setSize(75.f, 20.f);
	clear->setPosition(810.f, 360.f);
	clear->onPress(functions::clearGraph, &GC);

	auto matching = tgui::Button::create("Matching");
	v.emplace_back(matching, "Matching");
	matching->setSize(75.f, 20.f);
	matching->setPosition(810.f, 30.f);
	matching->onPress(functions::matching, &GC);
}

void update(tgui::Gui &gui, vector<pair<tgui::Button::Ptr, string>> &botoes,
			GraphCanvas &GC) {
	clear(gui, botoes);
	if (GC.GD.G.getN()) {
		general(gui, botoes);
		if (GC.GD.G.isBipartite()) bipartite(gui, botoes);
		if (GC.GD.G.isChordal()) chordal(gui, botoes);
		if (GC.GD.G.isDag()) dag(gui, botoes);
		if (GC.GD.G.isTree()) tree(gui, botoes);
	}
}
} // namespace buttons
