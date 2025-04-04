#ifndef INTERFACEH
#define INTERFACEH

#include "Buttons.hpp"
#include "Graph.hpp"
#include "GraphCanvas.hpp"
#include "Vector.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <chrono>
#include <string>
#include <thread>
using namespace std;

namespace interface {

Graph display(int, int, Graph = Graph());
}

#endif
