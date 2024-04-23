#pragma once

#include "Simulation.h"


class Buffon : public Simulation {
private:
	float m_width;
	float m_needleLength;
	int m_intersectingNeedles;
	sf::VertexArray m_needles{ sf::Lines };
	float m_piApprox;

	sf::RectangleShape m_background;
	sf::VertexArray m_lines{ sf::Lines };

	std::string_view m_unformattedText;
	TextWrap m_text;

protected:
	virtual void initialize() override;

	float getRandomNumber(float min, float max);

public:
	Buffon(sf::RenderWindow* window);

	virtual bool handleInputs(const sf::Event& sfmlEvent) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};
