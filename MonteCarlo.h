#pragma once

#include "Simulation.h"


class MonteCarlo : public Simulation {
private:
	sf::RectangleShape m_square;
	sf::CircleShape m_circle;
	sf::VertexArray m_points{ sf::Points };
	std::string_view m_unfomattedText;
	TextWrap m_text;

	int m_totalPoints = 0;
	int m_pointsInCircle = 0;
	float m_piApprox = 0.0f;

private:
	int getRandomInt(int a, int b);

protected:
	virtual void initialize() override;

public:
	MonteCarlo(sf::RenderWindow* m_window);

	virtual bool handleInputs(const sf::Event& sfmlEvent) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};
