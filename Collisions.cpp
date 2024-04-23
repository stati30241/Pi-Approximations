#include "Collisions.h"


Collisions::Collisions(sf::RenderWindow* window)
	: Simulation{ window, "Counting Block Collisions", "images/collisions.png" } {
	m_accuracySlider = SliderInt{ {630.0f, 30.0f}, {460.0f, 30.0f}, "Number of Digits", 1, 5 };
	initialize();
}


void Collisions::initialize() {
	m_background.clear();
	m_numCollisions = 0;

	m_background.append({ {50.0f, 0.0f} });
	m_background.append({ {50.0f, 600.0f} });
	m_background.append({ {50.0f, 600.0f} });
	m_background.append({ {1200.0f, 600.0f} });
	for (float i = 0.0f; i < 600.0f; i += 50.0f) {
		m_background.append({ {50.0f, i} });
		m_background.append({ {14.64f, i + 35.36f} });
	}

	m_smallBlock.rect.setSize({ 100.0f, 100.0f });
	m_smallBlock.rect.setPosition({ 300.0f, 500.0f });
	m_smallBlock.rect.setFillColor({ 66, 135, 245, 50 });
	m_smallBlock.rect.setOutlineColor({ 100, 100, 100 });
	m_smallBlock.rect.setOutlineThickness(-3.0f);
	m_smallBlock.velocity = 0.0f;
	m_smallBlock.mass = 1.0f;

	m_largeBlock.rect.setSize({ 200.0f, 200.0f });
	m_largeBlock.rect.setPosition({ 900.0f, 400.0f });
	m_largeBlock.rect.setFillColor({ 148, 78, 78, 50 });
	m_largeBlock.rect.setOutlineColor({ 100, 100, 100 });
	m_largeBlock.rect.setOutlineThickness(-3.0f);
	m_largeBlock.velocity = -100.0f;
	m_largeBlock.mass = std::pow(100, m_accuracySlider.getValue() - 1);

	m_unformattedText =
		"Collisions: {}\n\n"
		"Mass of Block A: {}\n"
		"Mass of Block B: {}\n\n"
		"Due to the elliptical shape of the equations that describe the conservation "
		"of momentum and kinetic energy, the number of collisions between the blocks "
		"and the wall correspond to the digits of pi.";
	m_text = TextWrap{ std::vformat(m_unformattedText, 
		std::make_format_args(m_numCollisions, m_smallBlock.mass, m_largeBlock.mass)), 20u, 1060.0f, 20.0f };
	m_text.setPosition({ 70.0f, 100.0f });

	m_soundBuffer.loadFromFile("clack.wav");
	m_clackSound.setBuffer(m_soundBuffer);
	m_clackSound.setPlayingOffset(sf::seconds(2));

	m_navbarPos = { 70.0f , 30.0f };
	Simulation::initializeNavbar();
}


bool Collisions::handleInputs(const sf::Event& sfmlEvent) {
	m_accuracySlider.handleInputs(sfmlEvent);
	float largeBlockMass = std::pow(100, m_accuracySlider.getValue() - 1);
	if (largeBlockMass != m_largeBlock.mass) {
		m_isPlaying = false;
		initialize();
		return true;
	}

	return Simulation::handleInputs(sfmlEvent);
}


void Collisions::update(float deltaTime) {
	if (!m_isPlaying) return;

	size_t timeSteps = std::pow(2, m_accuracySlider.getValue() - 1);
	float dt = deltaTime / timeSteps;

	for (size_t i = 0; i < timeSteps; ++i) {
		m_smallBlock.rect.move(m_smallBlock.velocity * dt, 0.0f);
		m_largeBlock.rect.move(m_largeBlock.velocity * dt, 0.0f);

		if (m_smallBlock.rect.getGlobalBounds().intersects(m_largeBlock.rect.getGlobalBounds())) {
			float temp = m_largeBlock.velocity;
			m_largeBlock.velocity = (m_smallBlock.mass * m_smallBlock.velocity + m_largeBlock.mass * m_largeBlock.velocity
				- m_smallBlock.mass * m_largeBlock.velocity + m_smallBlock.mass * m_smallBlock.velocity)
				/ (m_smallBlock.mass + m_largeBlock.mass);
			m_smallBlock.velocity = temp + m_largeBlock.velocity - m_smallBlock.velocity;
			m_smallBlock.rect.setPosition(m_largeBlock.rect.getPosition().x - m_smallBlock.rect.getSize().x,
				m_smallBlock.rect.getPosition().y);
			m_numCollisions++;

			m_clackSound.stop();
			m_clackSound.play();
		}

		if (m_smallBlock.rect.getPosition().x <= 50.0f) {
			m_smallBlock.velocity *= -1;
			m_smallBlock.rect.setPosition(50.0f, m_smallBlock.rect.getPosition().y);
			m_numCollisions++;

			m_clackSound.stop();
			m_clackSound.play();
		}
	}

	m_text.setString(std::vformat(m_unformattedText, std::make_format_args(
		m_numCollisions, m_smallBlock.mass, m_largeBlock.mass)));
}


void Collisions::render() {
	m_window->draw(m_smallBlock.rect);
	m_window->draw(m_largeBlock.rect);
	m_window->draw(m_background);
	m_window->draw(m_text);
	m_window->draw(m_accuracySlider);

	Simulation::render();
}
