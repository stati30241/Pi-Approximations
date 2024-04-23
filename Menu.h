#pragma once

#include "Simulation.h"


class Menu {
private:
	sf::RenderWindow* m_window;
	std::vector<Simulation*> m_simulations;
	std::vector<Button*> m_buttons;

public:
	Menu(sf::RenderWindow* window, const std::vector<Simulation*>& simulations);
	~Menu();

	Simulation* handleInputs(const sf::Event& sfmlEvent);
	void render();
};
