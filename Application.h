#pragma once

#include "Menu.h"
#include "MonteCarlo.h"
#include "Collisions.h"
#include "Polygons.h"
#include "Buffon.h"


// Main class
class Application {
private:
	sf::RenderWindow* m_window;
	sf::Clock m_clock;
	Menu* m_menu;
	Simulation* m_currentSimulation = nullptr;

private:
	void initialize();

	void handleInputs();
	void update();
	void render();

public:
	Application();
	~Application();

	void run();
};
