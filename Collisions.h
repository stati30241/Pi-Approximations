#pragma once

#include "Simulation.h"


struct Block {
	sf::RectangleShape rect;
	float velocity;
	float mass;
};


class Collisions : public Simulation {
private:
	sf::VertexArray m_background{ sf::Lines };
	Block m_smallBlock;
	Block m_largeBlock;
	SliderInt m_accuracySlider;
	std::string_view m_unformattedText;
	TextWrap m_text;

	sf::SoundBuffer m_soundBuffer;
	sf::Sound m_clackSound;

	size_t m_numCollisions;
	
protected:
	virtual void initialize() override;

public:
	Collisions(sf::RenderWindow* window);

	virtual bool handleInputs(const sf::Event& sfmlEvent) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};
