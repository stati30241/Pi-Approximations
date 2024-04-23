#include "Menu.h"


Menu::Menu(sf::RenderWindow* window, const std::vector<Simulation*>& simulations)
	: m_window{ window }, m_simulations{ simulations } {
	m_buttons.reserve(m_simulations.size());
	for (size_t i = 0; i < m_simulations.size(); ++i) {
		sf::Vector2f buttonPosition;
		buttonPosition.x = (i % 4) * m_window->getSize().x / 4.0f + 50.0f;
		buttonPosition.y = (i / 4) * m_window->getSize().y / 2.0f + 43.75f;

		Button* button = new Button{ buttonPosition, {200.0f, 250.0f} };
		button->setImage(Image{ m_simulations.at(i)->getImageFilename(), {12.0f, 20.0f}, {176.0f, 99.0f}});

		TextWrap buttonText{ m_simulations.at(i)->getName(), 20u, 176.0f, 20u };
		buttonText.setPosition({ 12.0f, 139.0f });
		button->setText(buttonText);

		m_buttons.push_back(button);
	}
}


Menu::~Menu() {
	for (size_t i = 0; i < m_simulations.size(); ++i) {
		delete m_simulations.at(i);
		delete m_buttons.at(i);
	}
}


Simulation* Menu::handleInputs(const sf::Event& sfmlEvent) {
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		m_buttons.at(i)->handleInputs(sfmlEvent);
		if (m_buttons.at(i)->isClicked()) return m_simulations.at(i);
	}

	return nullptr;
}


void Menu::render() {
	for (const Button* button : m_buttons) {
		m_window->draw(*button);
	}
}
