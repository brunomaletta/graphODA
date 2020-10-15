#include "Interface.hpp"

namespace interface {

sf::RenderWindow janela;
tgui::Gui gui;
GraphCanvas GC;
sf::Font fonte;
bool mudou;
bool para;

void lerGrafoArquivoAux(tgui::EditBox::Ptr arq) {
	Graph i;
	try {
		i = lerGrafoArquivo(arq->getText().toAnsiString());
	} catch (...) {
		return;
	}
	GC.setGraph(i);
	mudou = true;
}

void SaveScreen(tgui::ChildWindow::Ptr jan, tgui::EditBox::Ptr arq) {
	gui.remove(jan);
	janela.clear(sf::Color(251, 251, 251));
	GC.display();
	gui.draw();
	janela.display();

	sf::Vector2u windowSize = janela.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(janela);
	sf::Image screenshot = texture.copyToImage();

	// recortando só a parte necessária
	sf::Texture t;
	sf::IntRect r1(0, 0, 800, 600);
	t.create(800, 600);
	t.loadFromImage(screenshot, r1);
	sf::Image nova = t.copyToImage();
	bool ok = nova.saveToFile(arq->getText().toAnsiString() + ".png");
}

void Save2File() {
	auto jan = tgui::ChildWindow::create("Salvar");
	jan->setPosition(100.f, 100.f);
	jan->setSize(250.f, 50.f);
	auto texto = tgui::EditBox::create();
	texto->setDefaultText("Nome do arquivo");
	texto->setSize(200.f, 20.f);
	texto->setPosition(10.f, 10.f);
	jan->add(texto);

	// TODO: ERRO SE O TEXTO FOR VAZIO
	auto botao = tgui::Button::create("Ok");
	botao->setPosition(220.f, 10.f);
	botao->setSize(20.f, 20.f);
	botao->connect("pressed", SaveScreen, jan, texto);
	jan->add(botao);

	gui.add(jan);
}

void tikzVai3(tgui::ChildWindow::Ptr jan, tgui::EditBox::Ptr arq,
			  string *arquivo) {
	gui.remove(jan);
	janela.clear(sf::Color(251, 251, 251));
	GC.display();
	gui.draw();
	janela.display();

	GC.GD.getTikz(*arquivo, stod(arq->getText().toAnsiString()));
	para = false;
}

void tikzVai2(tgui::ChildWindow::Ptr jan, tgui::EditBox::Ptr arq) {
	gui.remove(jan);
	janela.clear(sf::Color(251, 251, 251));
	GC.display();
	gui.draw();
	janela.display();

	//static string arquivo = arq->getText().toAnsiString();
	static string arquivo;
	arquivo = arq->getText().toAnsiString();

	auto jan2 = tgui::ChildWindow::create("Escala");
	jan2->setPosition(100.f, 100.f);
	jan2->setSize(250.f, 50.f);
	auto texto = tgui::EditBox::create();
	texto->setDefaultText("Escala da imagem");
	texto->setSize(200.f, 20.f);
	texto->setPosition(10.f, 10.f);
	jan2->add(texto);

	// TODO: ERRO SE O TEXTO FOR VAZIO
	auto botao = tgui::Button::create("Ok");
	botao->setPosition(220.f, 10.f);
	botao->setSize(20.f, 20.f);
	botao->connect("pressed", tikzVai3, jan2, texto, &arquivo);
	jan2->add(botao);

	gui.add(jan2);
}

void tikzVai() {
	para = true;
	auto jan = tgui::ChildWindow::create("Tikz");
	jan->setPosition(100.f, 100.f);
	jan->setSize(250.f, 50.f);
	auto texto = tgui::EditBox::create();
	texto->setDefaultText("Nome do arquivo");
	texto->setSize(200.f, 20.f);
	texto->setPosition(10.f, 10.f);
	jan->add(texto);

	// TODO: ERRO SE O TEXTO FOR VAZIO
	auto botao = tgui::Button::create("Ok");
	botao->setPosition(220.f, 10.f);
	botao->setSize(20.f, 20.f);
	botao->connect("pressed", tikzVai2, jan, texto);
	jan->add(botao);

	gui.add(jan);
}

void Help() {
	auto jan = tgui::ChildWindow::create("Ajuda");
	jan->setPosition(100.f, 100.f);
	jan->setSize(250.f, 250.f);
	auto texto = tgui::MessageBox::create("Texto");
	texto->setPosition(0.f, 0.f);
	texto->setSize(10.f, 10.f);
	sf::String tipo(
		"Para carregar um grafo, \nutilize o botao \"importar\"\n\nMais ajudas "
		"coming soon :p");
	texto->setText(tipo);

	jan->add(texto);
	gui.add(jan);
}

void mudaDir() { (GC.GD.temDir) ^= 1; }

void centraliza() { (GC.GD.centr) ^= 1; }

void toggleDraw() {
	(GC.GD.draw ^= 1);
	para = false;
}

void reseta() {
	GC.setGraph(Graph());
	mudou = true;
}

void loadWidgets() {
	// tgui::Theme tema{"assets/TransparentGrey.txt"};
	// tgui::ButtonRenderer(tema.getRenderer("button")).setBackgroundColor(sf::Color::Blue);

	// Caixa de texto para o arquivo
	auto textoArquivo = tgui::EditBox::create();
	textoArquivo->setSize(200.f, 20.f);
	textoArquivo->setPosition(120.f, 615.f);
	textoArquivo->setDefaultText("Nome do arquivo");
	gui.add(textoArquivo);

	// Check box de se tem peso ou não
	auto check = tgui::CheckBox::create("Direcionado");
	check->setSize(20.f, 20.f);
	check->setPosition(120.f, 640.f);
	gui.add(check);

	// Check box de draw mode
	auto checkDraw = tgui::CheckBox::create("Editar");
	checkDraw->setSize(20.f, 20.f);
	checkDraw->setPosition(700.f, 615.f);
	gui.add(checkDraw);

	// Botão de help
	auto botaoHelp = tgui::Button::create("Help");
	botaoHelp->setSize(75.f, 20.f);
	botaoHelp->setPosition(20.f, 615.f);
	gui.add(botaoHelp);

	// botão de salvar
	auto botaoSave = tgui::Button::create("Save");
	botaoSave->setSize(75.f, 20.f);
	botaoSave->setPosition(20.f, 660.f);
	gui.add(botaoSave);

	// Botao pra ler arquivo
	auto botaoArquivo = tgui::Button::create("Importar");
	botaoArquivo->setSize(75.f, 20.f);
	botaoArquivo->setPosition(330.f, 615.f);
	gui.add(botaoArquivo);

	// botao pra centralizar grafo
	auto botaoCenter = tgui::Button::create("Centralizar");
	botaoCenter->setSize(75.f, 20.f);
	botaoCenter->setPosition(245.f, 640.f);
	// auto oi = tgui::Label::create("oi");
	// botaoCenter->setToolTip(oi);
	gui.add(botaoCenter);

	// botao pra resetar o grafo
	auto reset = tgui::Button::create("Reset");
	reset->setSize(75.f, 20.f);
	reset->setPosition(700.f, 640.f);
	gui.add(reset);

	// botao tikz
	auto tikz = tgui::Button::create("tikz");
	tikz->setSize(75.f, 20.f);
	tikz->setPosition(330.f, 640.f);
	gui.add(tikz);

	// Chama a função de importar arquivo
	botaoArquivo->connect("pressed", lerGrafoArquivoAux, textoArquivo);
	check->connect("checked", mudaDir);
	check->connect("unchecked", mudaDir);

	checkDraw->connect("checked", toggleDraw);
	checkDraw->connect("unchecked", toggleDraw);

	botaoHelp->connect("pressed", Help);
	botaoSave->connect("pressed", Save2File);
	botaoCenter->connect("pressed", centraliza);
	reset->connect("pressed", reseta);
	tikz->connect("pressed", tikzVai);
}

void drawStuff() {
	// Limpa tela e coloca branco
	janela.clear(sf::Color(251, 251, 251));

	// menu lateral
	sf::RectangleShape menu(sf::Vector2f(200.f, 700.f));
	menu.setFillColor(sf::Color(34, 38, 41));
	menu.setPosition(800.f, 0.f);
	janela.draw(menu);

	// Menu inferior
	sf::RectangleShape inferior(sf::Vector2f(800.f, 100.f)); // larg x alt
	inferior.setFillColor(sf::Color(34, 38, 41));
	inferior.setPosition(0.f, 600.f);
	janela.draw(inferior);

	// Contornos
	// lateral e canvas
	sf::Vertex linha0[] = {
		sf::Vertex(sf::Vector2f(1, 600), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(800, 600), sf::Color(71, 75, 79))};
	janela.draw(linha0, 10, sf::Lines);
	// superior e lateral
	sf::Vertex linha1[] = {
		sf::Vertex(sf::Vector2f(800, 0), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(1200, 0), sf::Color(71, 75, 79))};
	janela.draw(linha1, 10, sf::Lines);
	// borda inferior
	sf::Vertex linha2[] = {
		sf::Vertex(sf::Vector2f(0, 699), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(1200, 699), sf::Color(71, 75, 79))};
	janela.draw(linha2, 10, sf::Lines);
	// borda lateral esq
	sf::Vertex linha3[] = {
		sf::Vertex(sf::Vector2f(0, 600), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(0, 700), sf::Color(71, 75, 79))};
	janela.draw(linha3, 10, sf::Lines);
	// borda lateral dir
	sf::Vertex linha4[] = {
		sf::Vertex(sf::Vector2f(1199, 0), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(1199, 700), sf::Color(71, 75, 79))};
	janela.draw(linha4, 10, sf::Lines);
	// Divisão entre o canvas e o menu
	sf::Vertex linha7[] = {
		sf::Vertex(sf::Vector2f(800, 0), sf::Color(71, 75, 79)),

		sf::Vertex(sf::Vector2f(800, 700), sf::Color(71, 75, 79))};
	janela.draw(linha7, 10, sf::Lines);
}

void drawDrawMode(int X) {
	sf::Text draw;
	draw.setFont(fonte);
	draw.setString("E");
	draw.setCharacterSize(32);
	draw.setFillColor(sf::Color(134, 194, 50));
	draw.setPosition(X - 32, 0);
	janela.draw(draw);
}

Graph display(int X, int Y, Graph G) {
	// Carrega a fonte Consola Bold (Gosto dela)
	if (!fonte.loadFromFile("assets/CONSOLAB.TTF")) {
		// TODO: Erro direito
		return G;
	}

	// Antialiasing
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	// Cria a janela

	janela.create(sf::VideoMode(X, Y), "graphODA", sf::Style::Close, settings);
	// janela = &janela2;

	// janela.setKeyRepeatEnabled(false);
	// gui = tgui::Gui(*janela);
	gui.setTarget(janela);
	tgui::Theme tema{"assets/Botoes.txt"};
	tgui::Theme::setDefault(&tema);

	// GraphCanvas
	GC = GraphCanvas(janela, fonte, X * 4 / 5, Y * 6 / 7);
	GC.setGraph(G);
	bool editing;
	para = false;
	tgui::EditBox::Ptr edit;

	// Tenta importar os widgets da gui
	try {
		loadWidgets();
	} catch (const tgui::Exception &e) {
		// TODO: mensagem de erro
		return G;
	}

	// Botoes
	vector<tgui::Button::Ptr> botoes;
	buttons::init(botoes, GC);
	buttons::update(gui, botoes, GC);

	// "Main Loop"
	// Roda o programa enquanto a janela estiver aberta

	while (janela.isOpen()) {
		// dorme um pouco para nao usar muito processador
		this_thread::sleep_for(chrono::milliseconds(1));

		// se janela nao ta ativa, espera
		if (!janela.hasFocus()) continue;

		// Checa se algum evento aconteceu
		sf::Event evento;
		while (janela.pollEvent(evento)) {
			// Se pediu pra fechar
			if (evento.type == sf::Event::Closed) janela.close();

			// Cria os widgets da GUI
			gui.handleEvent(evento);

			// apertou alguma coisa no teclado
			if (evento.type == sf::Event::KeyPressed) {
				// ctrl -> toggle draw mode
				if (0 and evento.key.code == sf::Keyboard::LControl and
					!editing)
					GC.GD.draw ^= 1;

				// esc -> sai da edicao dos pesos/labels
				if (evento.key.code == sf::Keyboard::Escape) {
					editing = 0;
					gui.remove(edit);
					GC.editLabel = GC.editWeight = -1;
				}

				// enter -> edita peso/label de fato
				if (evento.key.code == sf::Keyboard::Return and editing) {
					// label
					if (GC.editLabel > -1)
						GC.GD.G.label[GC.editLabel] =
							edit->getText().toAnsiString();

					// weight
					if (GC.editWeight > -1) {
						string s = edit->getText().toAnsiString();

						bool valid = 1;
						if (!s.size())
							valid = 0;
						else if (!(s[0] == '-' or
								   (s[0] >= '0' and s[0] <= '9')))
							valid = 0;
						for (int i = 1; i < s.size(); i++)
							if (s[i] < '0' or s[i] > '9') valid = 0;

						if (valid) {
							Graph G(GC.GD.G.getN());
							G.label = GC.GD.G.label;
							int position = 0;
							auto adj = GC.GD.G.getAdj();
							for (int i = 0; i < GC.GD.G.getN(); i++)
								for (auto &j : adj[i]) {
									if (position++ == GC.editWeight)
										G.addEdge(i, j.first,
												  stoi(edit->getText()
														   .toAnsiString()));
									else
										G.addEdge(i, j.first, j.second);
								}
							GC.GD.setGraphContinue(G);
						} else {
							// TODO: erro direito
							cout << "ERRO: peso invalido" << endl;
						}
					}

					editing = 0;
					gui.remove(edit);
					GC.editLabel = GC.editWeight = -1;
				}
			}
		}

		drawStuff();

		// Tipo do grafo
		sf::Text tipo;
		tipo.setFont(fonte);
		tipo.setCharacterSize(18);
		tipo.setFillColor(sf::Color(134, 194, 50));
		tipo.setPosition(810, 605);

		string msg = "";
		if (GC.GD.G.isBipartite()) msg += "Bipartido\n";
		if (GC.GD.G.isChordal()) msg += "Cordal\n";
		if (GC.GD.G.isDag()) msg += "DAG\n";
		if (GC.GD.G.isTree()) msg += "Arvore\n";
		if (!GC.GD.G.getN()) msg = "";
		tipo.setString(msg);

		janela.draw(tipo);

		/*
		97,137,47 - verde escuro 61892F
		134,194,50 - verde claro 86c232
		34,38,41 - black 222629
		71,75,79 - grey 474b4f
		107,110,112 - light gray 6b6e70
		251,251,251 - snow FBFBFB
		*/

		if (GC.GD.draw) drawDrawMode(X * 4 / 5);

		// olha se ta editando os pesos/labels
		if (GC.editLabel > -1 or GC.editWeight > -1) {
			if (!editing) {
				editing = 1;
				auto edg = GC.GD.G.getEdges();

				if (GC.editLabel > -1) {
					edit = tgui::EditBox::create();
					edit->setSize(GC.GD.raio * sqrt(2), GC.GD.raio * sqrt(2));
					edit->setPosition(
						GC.GD.pos[GC.editLabel].x - (GC.GD.raio - 1) / sqrt(2),
						GC.GD.pos[GC.editLabel].y - (GC.GD.raio - 1) / sqrt(2));
					edit->setText(GC.GD.G.label[GC.editLabel]);
					gui.add(edit);
					edit->setFocused(true);
				}

				if (GC.editWeight > -1) {
					edit = tgui::EditBox::create();
					Vector size(GC.GD.raio * sqrt(2), GC.GD.raio * sqrt(2));
					edit->setSize(size.x, size.y);

					// acha posicao
					Vector ini = GC.GD.pos[edg[GC.editWeight].first],
						   fim = GC.GD.pos[edg[GC.editWeight].second];
					Vector pos =
						ini + (fim - ini) * GC.GD.posPeso[GC.editWeight];

					// arestas paralelas
					Vector add(0, 0);
					if (GC.GD.isParal[GC.editWeight]) {
						add = fim - ini;
						if (add.norm()) {
							add = add * (1 / add.norm());
							add = add.rotate(acos(-1.0) / 2);
							add = add * (GC.GD.raio / 3.0);
						}
					}
					pos = pos + add;

					edit->setPosition(pos.x - size.x / 2, pos.y - size.y / 2);
					int peso, position = 0;
					auto adj = GC.GD.G.getAdj();
					for (int i = 0; i < GC.GD.G.getN(); i++)
						for (auto j : adj[i])
							if (position++ == GC.editWeight) peso = j.second;
					edit->setText(to_string(peso));
					gui.add(edit);
					edit->setFocused(true);
				}
			}
		} else if (!para and (GC.handleClique() or mudou)) {
			buttons::update(gui, botoes, GC);
			mudou = false;
		}

		// views do grafo
		GC.display();

		gui.draw();

		// Termina iteração e atualiza janela
		janela.display();
	}

	return GC.GD.G;
}
} // namespace interface
