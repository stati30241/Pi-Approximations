#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <format>
#include <numbers>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_map>


class FontManager {
public:
	sf::Font font;

private:
	FontManager(std::string_view filename) {
		font.loadFromFile(filename.data());
	}

	FontManager(const FontManager& copy);
	FontManager operator= (const FontManager& copy);

public:
	static FontManager& getFont(std::string_view filename = "") {
		static FontManager instance{ filename };
		return instance;
	}
};


class GUI : public sf::Drawable, public sf::Transformable {
public:
	virtual void handleInputs(const sf::Event& smflEvent) = 0;
};


class TextWrap : public GUI {
private:
	std::vector<sf::Text> m_text;
	std::string m_string;

	size_t m_fontSize;
	float m_maxWidth;
	float m_lineSpacing;
	sf::Color m_color;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	TextWrap() : m_fontSize{ 0 }, m_maxWidth{ 0.0f }, m_lineSpacing{ 0.0f } {}
	TextWrap(std::string_view string, size_t fontSize, float maxWidth,
		float lineSpacing, const sf::Color& color = sf::Color::White);

	std::string getString() const { return m_string; }
	void setString(std::string_view string);
	float getHeight() const { return m_text.empty() ? 0.0f : m_lineSpacing * m_text.size(); }
	float getWidth() const { return std::max_element(m_text.begin(), m_text.end(), [](const sf::Text& a, const sf::Text& b)
		{ return a.getGlobalBounds().width > b.getGlobalBounds().width; })->getGlobalBounds().width; }

	virtual void handleInputs(const sf::Event& sfmlEvent) override {}
};


class Image : public GUI {
private:
	sf::Texture* m_texture;
	sf::VertexArray m_vertecies{ sf::Quads, 4 };
	sf::Vector2f m_size;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Image() : m_texture{ nullptr }, m_size{ 0, 0 } { }
	Image(std::string_view filename, const sf::Vector2f& position, const sf::Vector2f& size);
	~Image();

	Image(const Image& other);
	Image& operator= (Image other);

	sf::Vector2f getSize() const { return m_size; }
	void setSize(const sf::Vector2f& size);

	virtual void handleInputs(const sf::Event& sfmlEvent) override {}
};


class Button : public GUI {
private:
	sf::RectangleShape m_buttonRect;
	sf::Color m_defaultColor;
	sf::Color m_highlightedColor;

	TextWrap m_text;
	Image m_image;

	bool m_isClicked = false;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Button(const sf::Vector2f& position, const sf::Vector2f& size);
	Button(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text);
	Button(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view imageFilename, const sf::Vector2f& imageSize);
	
	void setText(const TextWrap& text) { m_text = text; }
	void setImage(const Image& image) { m_image = image; }
	void setDefaultColor(const sf::Color& color) { m_defaultColor = color; }
	void setHighlightedColor(const sf::Color& color) { m_highlightedColor = color; }
	void setOutlineColor(const sf::Color& color) { m_buttonRect.setOutlineColor(color); }

	virtual void handleInputs(const sf::Event& sfmlEvent) override;
	bool isClicked();
};


template <typename T>
class Slider : public GUI {
protected:
	sf::RectangleShape m_sliderRect;
	sf::RectangleShape m_slideBarRect;
	sf::Color m_defaultColor;
	sf::Color m_highlightedColor;
	bool m_isDragging = false;

	TextWrap m_text;
	TextWrap m_valueText;

	T m_min, m_max;
	T m_value;
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	virtual void updateValue(float delta)  = 0;

public:
	Slider() {}
	Slider(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text, T min, T max);

	T getValue() { return m_value; }
	void setValue(T value);

	virtual void handleInputs(const sf::Event& sfmlEvent) override;
};


template <typename T>
Slider<T>::Slider(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text, T min, T max)
	: m_min{ min }, m_max{ max }, m_value{ min } {
	setPosition(position);
	m_sliderRect.setSize(size);
	m_sliderRect.setOutlineThickness(5);

	m_slideBarRect.setSize({ size.x / 10.0f, size.y * 1.20f });
	m_slideBarRect.setOrigin(m_slideBarRect.getSize().x / 2.0f, 0.0f);
	m_slideBarRect.setPosition(0.0f, -size.y * 0.10f);
	m_slideBarRect.setOutlineThickness(5);

	m_defaultColor = sf::Color{ 100, 100, 100, 100 };
	m_highlightedColor = sf::Color{ 100, 100, 100, 255 };
	m_sliderRect.setFillColor(m_defaultColor);
	m_sliderRect.setOutlineColor(sf::Color{ 30, 30, 30 });
	m_slideBarRect.setFillColor(m_highlightedColor);
	m_slideBarRect.setOutlineColor({ sf::Color{ 30, 30, 30 } });

	m_text = TextWrap{ text, 20u, size.x - 5.0f, 20u };
	m_text.setPosition(5.0f, size.y * 1.4f);

	m_valueText = TextWrap{ std::to_string(m_value), 20u, size.x, 20u };
	m_valueText.setPosition((size.x - m_valueText.getWidth()) / 2.0f, (size.y - m_valueText.getHeight()) / 2.0f);
}


template <typename T>
void Slider<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_sliderRect, states);
	target.draw(m_slideBarRect, states);
	target.draw(m_text, states);
	target.draw(m_valueText, states);
}

template <typename T>
void Slider<T>::setValue(T value) {
	if (value < m_min || value > m_max) return;
	m_value = value;
	float delta = (m_value - m_min) * m_sliderRect.getSize().x / (m_max - m_min);
	m_slideBarRect.setPosition(delta, m_slideBarRect.getPosition().y);
}


template <typename T>
void Slider<T>::handleInputs(const sf::Event& sfmlEvent) {
	switch (sfmlEvent.type) {
	case sf::Event::MouseButtonPressed:
		if (sfmlEvent.mouseButton.x >= getPosition().x && sfmlEvent.mouseButton.x <= getPosition().x + m_sliderRect.getSize().x &&
			sfmlEvent.mouseButton.y >= getPosition().y && sfmlEvent.mouseButton.y <= getPosition().y + m_sliderRect.getSize().y) {
			updateValue(sfmlEvent.mouseButton.x - getPosition().x);
			m_isDragging = true;
		} break;

	case sf::Event::MouseButtonReleased:
		m_isDragging = false;
		break;

	case sf::Event::MouseMoved:
		if (sfmlEvent.mouseMove.x >= getPosition().x && sfmlEvent.mouseMove.x <= getPosition().x + m_sliderRect.getSize().x &&
			sfmlEvent.mouseMove.y >= getPosition().y && sfmlEvent.mouseMove.y <= getPosition().y + m_sliderRect.getSize().y) {
			m_sliderRect.setFillColor(m_highlightedColor);
			if (m_isDragging) {
				updateValue(sfmlEvent.mouseMove.x - getPosition().x);
			}
		} else {
			m_sliderRect.setFillColor(m_defaultColor);
		} break;
	}

	m_valueText.setString(std::to_string(m_value));
	m_valueText.setPosition((m_sliderRect.getSize().x - m_valueText.getWidth()) / 2.0f,
		(m_sliderRect.getSize().y - m_valueText.getHeight()) / 2.0f);
}


class SliderFloat : public Slider<float> {
protected:
	void updateValue(float delta);

public:
	SliderFloat() {}
	SliderFloat(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text, float min, float max)
		: Slider<float>{ position, size, text, min, max } { }
};


class SliderInt : public Slider<int> {
protected:
	void updateValue(float delta);

public:
	SliderInt() {}
	SliderInt(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text, int min, int max)
		: Slider<int>{ position, size, text, min, max } { }
};
