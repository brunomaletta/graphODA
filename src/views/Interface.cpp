#include "Interface.hpp"

void lerGrafoArquivoAux(tgui::EditBox::Ptr arq, GraphCanvas *GC, bool *mudou) {
	Graph i;
	try {
		i = lerGrafoArquivo(arq->getText().toAnsiString());
	} catch (...) {
		return;
	}
	GC->setGraph(i);
	*mudou = true;
}

void mudaDir(GraphCanvas *GC) { (GC->GD.temDir) ^= 1; }

void centraliza(GraphCanvas *GC) { (GC->GD.centr) ^= 1; }

void toggleDraw(GraphCanvas *GC) { (GC->GD.draw ^= 1); }

void reseta(GraphCanvas *GC, bool* mudou) {
	GC->setGraph(Graph());
	*mudou = true;
}

void loadWidgets(tgui::Gui &gui, GraphCanvas *GC, bool *mudou) {
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
	gui.add(botaoCenter);

	// botao pra resetar o grafo
	auto reset = tgui::Button::create("Reset");
	reset->setSize(75.f, 20.f);
	reset->setPosition(700.f, 640.f);
	gui.add(reset);

	// Chama a função de importar arquivo
	botaoArquivo->connect("pressed", lerGrafoArquivoAux, textoArquivo, GC, mudou);
	check->connect("checked", mudaDir, GC);
	check->connect("unchecked", mudaDir, GC);

	checkDraw->connect("checked", toggleDraw, GC);
	checkDraw->connect("unchecked", toggleDraw, GC);
	
	botaoCenter->connect("pressed", centraliza, GC);
	reset->connect("pressed", reseta, GC, mudou);
}

void drawStuff(sf::RenderWindow &janela, sf::Font &fonte) {
	// Limpa tela e coloca branco
	janela.clear(sf::Color(251,251,251));

	// menu lateral
	sf::RectangleShape menu(sf::Vector2f(400.f, 700.f));
	menu.setFillColor(sf::Color(34,38,41));
	menu.setPosition(800.f, 0.f);
	janela.draw(menu);

	// Menu inferior
	sf::RectangleShape inferior(sf::Vector2f(800.f, 100.f)); // larg x alt
	inferior.setFillColor(sf::Color(34,38,41));
	inferior.setPosition(0.f, 600.f);
	janela.draw(inferior);
	
	// Contornos
		//lateral e canvas
	sf::Vertex linha0[] = {
		sf::Vertex(sf::Vector2f(1, 600), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(800, 600), sf::Color(71,75,79))};
	janela.draw(linha0, 10, sf::Lines);
		//superior e lateral
	sf::Vertex linha1[] = {
		sf::Vertex(sf::Vector2f(800, 0), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(1200, 0), sf::Color(71,75,79))};
	janela.draw(linha1, 10, sf::Lines);	
		//borda inferior
	sf::Vertex linha2[] = {
		sf::Vertex(sf::Vector2f(0, 699), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(1200, 699), sf::Color(71,75,79))};
	janela.draw(linha2, 10, sf::Lines);
		//borda lateral esq
	sf::Vertex linha3[] = {
		sf::Vertex(sf::Vector2f(0, 600), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(0, 700), sf::Color(71,75,79))};
	janela.draw(linha3, 10, sf::Lines);
		//borda lateral dir
	sf::Vertex linha4[] = {
		sf::Vertex(sf::Vector2f(1199, 0), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(1199, 700), sf::Color(71,75,79))};
	janela.draw(linha4, 10, sf::Lines);
		// Divisão entre o canvas e o menu
	sf::Vertex linha7[] = {
		sf::Vertex(sf::Vector2f(800, 0), sf::Color(71,75,79)),

		sf::Vertex(sf::Vector2f(800, 700), sf::Color(71,75,79))};
	janela.draw(linha7, 10, sf::Lines);
}

void drawDrawMode(sf::RenderWindow &janela, sf::Font &fonte, int X) {
	sf::Text draw;
	draw.setFont(fonte);
	draw.setString("D");
	draw.setCharacterSize(32);
	draw.setFillColor(sf::Color(134,194,50));
	draw.setPosition(X - 32, 0);
	janela.draw(draw);
}

Graph displayTeste(int X, int Y, Graph G) {
	// Carrega a fonte Consola Bold (Gosto dela)
	sf::Font fonte;
	if (!fonte.loadFromFile("assets/CONSOLAB.TTF")) {
		// TODO: Erro direito
		return G;
	}

	// Antialiasing
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	// Cria a janela
	sf::RenderWindow janela(sf::VideoMode(X, Y), "graphODA", sf::Style::Close,
							settings);
	janela.setKeyRepeatEnabled(false);
	tgui::Gui gui(janela);
	tgui::Theme tema{"assets/Botoes.txt"};
	tgui::Theme::setDefault(&tema);

	// GraphCanvas
	GraphCanvas GC(janela, fonte, X * 2 / 3, Y * 6 / 7, 15);
	GC.setGraph(G);
	bool editing;
	tgui::EditBox::Ptr edit;

	bool mudou = true;

	// Tenta importar os widgets da gui
	try {
		loadWidgets(gui, &GC, &mudou);
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
				if (0 and evento.key.code == sf::Keyboard::LControl and !editing)
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

		drawStuff(janela, fonte);
		
		// Tipo do grafo
		sf::Text tipo;
		tipo.setFont(fonte);
		tipo.setCharacterSize(18);
		tipo.setFillColor(sf::Color(134,194,50));
		tipo.setPosition(810, 605);

		string msg = "";
		if (GC.GD.G.isBipartite()) msg += "Bipartido\n";
		if (GC.GD.G.isChordal()) msg += "Cordal\n";
		if (GC.GD.G.isDag()) msg += "DAG\n";
		if (GC.GD.G.isTree()) msg += "Arvore\n";
		if(!GC.GD.G.getN()) msg = "";
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

		if (GC.GD.draw) drawDrawMode(janela, fonte, X * 2 / 3);

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
		} else if (GC.handleClique() or mudou) {
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
