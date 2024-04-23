#include "Buffon.h"


Buffon::Buffon(sf::RenderWindow* window)
	: Simulation{ window, "Buffon's Needle Problem", "images/buffon.png" } {
	initialize();
}


void Buffon::initialize() {
	m_width = 60.0f;
	m_needleLength = 60.0f;

	m_intersectingNeedles = 0;
	m_needles.clear();
	m_piApprox = 0.0f;

	m_background.setPosition({ 37.5f, 37.5f });
	m_background.setSize({ 600.0f, 600.0f });
	m_background.setFillColor({ 50, 50, 50, 150 });

	m_lines.clear();
	for (float i = 0; i <= m_background.getSize().x; i += m_width) {
		m_lines.append({ {37.5f + i, 37.5f}, sf::Color{ 100, 100, 100 }});
		m_lines.append({ {37.5f + i, 37.5f + m_background.getSize().y}, sf::Color{100, 100, 100}});
	}

	m_unformattedText = "Number of needles: {}\n"
		"Number of intersecting needles: {}\n\n"
		"Pi: {}\n\n"
		"A problem posed in the 18th century by Georges-Louis Leclerc, Comte de Buffon, where it "
		"is asked to calculate the probability of a randomly thrown needle landing on a line on "
		"a board with evenly spaced out lines. The solution to this problem includes pi, and so "
		"working backwards, you can extract an approximation for pi by randomy throwing needles "
		"on a lined board.";
	m_text = TextWrap{ std::vformat(m_unformattedText, std::make_format_args(m_needles.getVertexCount() / 2,
		m_intersectingNeedles, m_piApprox)), 20u, 460.0f, 20.0f };
	m_text.setPosition(688.75f, 120.0f);

	m_navbarPos = { 688.75f, 40.0f };
	Simulation::initializeNavbar();
}


float Buffon::getRandomNumber(float min, float max) {
	static std::mt19937 randomEngine{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_real_distribution<float> randDist{ min, max };
	return randDist(randomEngine);
}


bool Buffon::handleInputs(const sf::Event& sfmlEvent) {
	return Simulation::handleInputs(sfmlEvent);
}


void Buffon::update(float deltaTime) {
	if (!m_isPlaying) return;

	sf::Vector2f startPos = { getRandomNumber(37.5f, 637.5f), getRandomNumber(37.5f, 637.5f) };
	float angle = getRandomNumber(0.0f, 2.0f * std::numbers::pi);
	sf::Vector2f endPos = startPos + m_needleLength * sf::Vector2f{ std::cosf(angle), std::sinf(angle) };
	sf::Color color = sf::Color::Yellow;

	for (float i = 37.5f; i <= m_background.getSize().x + 37.5f; i += m_width) {
		if ((startPos.x < i && endPos.x > i) || (startPos.x > i && endPos.x < i)) {
			m_intersectingNeedles++;
			color = sf::Color::Green;
		}
	}

	if (endPos.x < 37.5f || endPos.x > 637.5f) {
		endPos.x = endPos.x < 37.5f ? 37.5 : 637.5;
		endPos.y = startPos.y + (endPos.x - startPos.x) * std::tanf(angle);
	} if (endPos.y < 37.5f || endPos.y > 637.5f) {
		endPos.y = endPos.y < 37.5f ? 37.5 : 637.5;
		endPos.x = startPos.x + (endPos.y - startPos.y) / std::tanf(angle);
	}
	
	m_needles.append({ startPos, color });
	m_needles.append({ endPos, color });

	float p = static_cast<double>(m_intersectingNeedles) / (m_needles.getVertexCount() / 2.0);
	m_piApprox = (2.0 * m_needleLength) / (static_cast<double>(m_width) * p);

	m_text.setString(std::vformat(m_unformattedText, std::make_format_args(m_needles.getVertexCount() / 2,
		m_intersectingNeedles, m_piApprox)));
}


void Buffon::render() {
	m_window->draw(m_background);
	m_window->draw(m_needles);
	m_window->draw(m_lines);
	m_window->draw(m_text);

	Simulation::render();
}
