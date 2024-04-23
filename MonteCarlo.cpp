#include "MonteCarlo.h"


MonteCarlo::MonteCarlo(sf::RenderWindow* window)
	: Simulation { window, "Monte Carlo Method", "images/montecarlo.png" } {
	initialize();
}


void MonteCarlo::initialize() {
	m_piApprox = 0.0f;
	m_totalPoints = 0;
	m_pointsInCircle = 0;

	m_square.setSize({ 600.0f, 600.0f });
	m_square.setPosition({ 37.5f, 37.5f });
	m_square.setOutlineThickness(2);
	m_square.setOutlineColor({ 100, 100, 100 });
	m_square.setFillColor({ 0, 0, 0, 30 });

	m_circle.setRadius(300.0f);
	m_circle.setPosition({ 37.5f, 37.5f });
	m_circle.setPointCount(60);
	m_circle.setOutlineThickness(2);
	m_circle.setOutlineColor({ 100, 100, 100 });
	m_circle.setFillColor(sf::Color::Transparent);

	m_points.clear();

	m_unfomattedText =
		"Pi: {}\n"
		"Total points: {}\n"
		"Points inside the circle: {}\n\n\n"
		"The ratio of the area of the circle of radius r and the area of a"
		"square with a side length 2r is pi/4. This method simulates calculating "
		"that area using random points that are picked. From then on the ratio "
		"of the points inside theW circle and total points is calculated to "
		"approximate the value of pi.";
	m_text = TextWrap{ std::vformat(m_unfomattedText, std::make_format_args(m_piApprox, m_totalPoints, m_pointsInCircle)),
		20u, 460.0f, 20};
	m_text.setPosition(688.75f, 100.0f);

	m_navbarPos = { 688.75f, 40.0f };
	initializeNavbar();
}


int MonteCarlo::getRandomInt(int a, int b) {
	static std::mt19937 randomEngine{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_int_distribution<int> randDist{ a, b };
	return randDist(randomEngine);
}


bool MonteCarlo::handleInputs(const sf::Event& sfmlEvent) {
	return Simulation::handleInputs(sfmlEvent);
}


void MonteCarlo::update(float deltaTime) {
	if (!m_isPlaying) return;

	sf::Vector2f pointPos;
	pointPos.x = getRandomInt(m_square.getPosition().x, m_square.getPosition().x + m_square.getSize().x);
	pointPos.y = getRandomInt(m_square.getPosition().y, m_square.getPosition().y + m_square.getSize().y);
	sf::Vector2f center = m_square.getPosition() + m_square.getSize() / 2.0f;
	sf::Color pointColor = std::powf(pointPos.x - center.x, 2) + std::powf(pointPos.y - center.x, 2) 
		< std::powf(m_square.getSize().x / 2.0f, 2) ? sf::Color::Green : sf::Color::White;
	m_points.append({ pointPos, pointColor });

	m_totalPoints++;
	if (pointColor == sf::Color::Green) m_pointsInCircle++;
	m_piApprox = 4.0f * m_pointsInCircle / m_totalPoints;
	m_text.setString(std::vformat(m_unfomattedText, std::make_format_args(m_piApprox, m_totalPoints, m_pointsInCircle)));
}


void MonteCarlo::render() {
	m_window->draw(m_square);
	m_window->draw(m_circle);
	m_window->draw(m_points);
	m_window->draw(m_text);

	Simulation::render();
}
