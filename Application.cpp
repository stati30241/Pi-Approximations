#include "Application.h"


Application::Application() {
	initialize();
}


Application::~Application() {
	delete m_window;
	delete m_menu;
}


void Application::initialize() {
	sf::VideoMode windowSize = { 1200u, 675u };
	sf::String windowTitle = "Pi Approximations";
	size_t windowStyle = sf::Style::Close;
	m_window = new sf::RenderWindow{ windowSize, windowTitle, windowStyle };

	m_menu = new Menu{ m_window, { new MonteCarlo{m_window }, new Collisions{ m_window },
		new Polygons{ m_window }, new Buffon{ m_window } } };
}


void Application::handleInputs() {
	sf::Event sfmlEvent{ };
	while (m_window->pollEvent(sfmlEvent)) {
		if (!m_currentSimulation) m_currentSimulation = m_menu->handleInputs(sfmlEvent);
		else if (!m_currentSimulation->handleInputs(sfmlEvent)) m_currentSimulation = nullptr;

		switch (sfmlEvent.type) {
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}
}


void Application::update() {
	float deltaTime = m_clock.restart().asSeconds();
	if (deltaTime >= 0.1f) deltaTime = 1.0f / 60.0f;

	if (!m_currentSimulation) return;
	else m_currentSimulation->update(deltaTime);
}


void Application::render() {
	m_window->clear({ 50, 40, 80 });

	if (m_currentSimulation) m_currentSimulation->render();
	else m_menu->render();

	m_window->display();
}


void Application::run() {
	while (m_window->isOpen()) {
		handleInputs();
		update();
		render();
	}
}
