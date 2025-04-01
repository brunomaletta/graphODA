#ifndef BUTTONSH
#define BUTTONSH

#include "GraphCanvas.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <string>
#include <vector>

using namespace std;

namespace buttons {
void init(vector<pair<tgui::Button::Ptr, string>>&, GraphCanvas&);
void update(tgui::Gui&, vector<pair<tgui::Button::Ptr, string>>&, GraphCanvas&);
} // namespace buttons

#endif
