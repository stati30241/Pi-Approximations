#include "GUI.h"


TextWrap::TextWrap(std::string_view string, size_t fontSize, float maxWidth, float lineSpacing, const sf::Color& color)
	: m_string{ string.data() }, m_fontSize{ fontSize }, m_maxWidth{ maxWidth },
	m_lineSpacing{ lineSpacing }, m_color{ color } {
	setString(m_string);
}


void TextWrap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (sf::Text text : m_text) {
		target.draw(text, states);
	}
}


void TextWrap::setString(std::string_view string) {
	m_text.clear();
	m_string = string.data();

	sf::Text line{ "" , FontManager::getFont("cour.ttf").font, m_fontSize };
	std::string token = "";
	for (char ch : m_string + " ") {
		if (ch == ' ') {
			sf::Text newLine{ "", FontManager::getFont("cour.ttf").font, m_fontSize };
			if (line.getString().isEmpty()) {
				line.setString(token);
				token = "";
				continue;
			} else newLine.setString(line.getString() + " " + token);

			if (newLine.getGlobalBounds().width > m_maxWidth) {
				m_text.push_back(line);
				line.setString(token);
			} else {
				line.setString(newLine.getString());
			}

			token = "";
		} else if (ch == '\n') {
			line.setString(line.getString() + " " + token);
			m_text.push_back(line);
			line.setString("");
			token = "";
		} else token += ch;
	}
	m_text.push_back(line);

	for (size_t i = 0; i < m_text.size(); ++i) {
		m_text.at(i).setPosition(0.0f, m_lineSpacing * i);
		m_text.at(i).setFillColor(m_color);
	}
}


Image::Image(std::string_view filename, const sf::Vector2f& position, const sf::Vector2f& size) {
	setPosition(position);
	setSize(size);

	m_texture = new sf::Texture{ };
	m_texture->loadFromFile(filename.data());
	m_vertecies[0].texCoords = { 0.0f, 0.0f };
	m_vertecies[1].texCoords = { static_cast<float>(m_texture->getSize().x), 0.0f };
	m_vertecies[2].texCoords = { static_cast<float>(m_texture->getSize().x), static_cast<float>(m_texture->getSize().y) };
	m_vertecies[3].texCoords = { 0.0f, static_cast<float>(m_texture->getSize().y) };
}


Image::~Image() {
	delete m_texture;
}


Image::Image(const Image& other) {
	m_size = other.m_size;
	m_vertecies = other.m_vertecies;
	setPosition(other.getPosition());
	m_texture = new sf::Texture{ *other.m_texture };
}

Image& Image::operator= (Image other) {
	std::swap(m_size, other.m_size);
	std::swap(m_vertecies, other.m_vertecies);
	setPosition(other.getPosition());
	std::swap(m_texture, other.m_texture);

	return *this;
}


void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = m_texture;
	target.draw(m_vertecies, states);
}


void Image::setSize(const sf::Vector2f& size) {
	m_size = size;

	m_vertecies[0].position = { 0.0f, 0.0f };
	m_vertecies[1].position = { m_size.x, 0.0f };
	m_vertecies[2].position = { m_size.x, m_size.y };
	m_vertecies[3].position = { 0.0f, m_size.y };
}


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size) {
	setPosition(position);
	m_buttonRect.setSize(size);
	m_buttonRect.setOutlineThickness(5);
	
	m_defaultColor = sf::Color{ 100, 100, 100, 100 };
	m_highlightedColor = sf::Color{ 100, 100, 100, 255 };
	m_buttonRect.setFillColor(m_defaultColor);
	m_buttonRect.setOutlineColor(sf::Color{ 30, 30, 30 });
}


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view text)
	: Button{ position, size } {
	m_text = TextWrap{ text, 20u, size.x * 0.9f, 20u };
	sf::Vector2f a = { m_text.getWidth(), 20.0f };
	m_text.setPosition(sf::Vector2f{ (size.x - m_text.getWidth()) / 2.0f,(size.y - a.y)/2.0f});
	//m_text.setPosition(sf::Vector2f{ 0.0f, 0.0f });

}


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, std::string_view imageFilename, const sf::Vector2f& imageSize)
	: Button{ position, size } {
	const sf::Vector2f imagePos = (size - imageSize) / 2.0f;
	m_image = Image{ imageFilename, imagePos + getPosition(), imageSize};
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_buttonRect, states);
	if (m_text.getHeight()) target.draw(m_text, states);
	if (m_image.getSize().x) target.draw(m_image, states);
}


void Button::handleInputs(const sf::Event& sfmlEvent) {
	switch (sfmlEvent.type) {
	case sf::Event::MouseButtonPressed:
		if (sfmlEvent.mouseButton.x >= getPosition().x && sfmlEvent.mouseButton.x <= getPosition().x + m_buttonRect.getSize().x &&
			sfmlEvent.mouseButton.y >= getPosition().y && sfmlEvent.mouseButton.y <= getPosition().y + m_buttonRect.getSize().y) {
			m_isClicked = true;
		} break;

	case sf::Event::MouseMoved:
		if (sfmlEvent.mouseMove.x >= getPosition().x && sfmlEvent.mouseMove.x <= getPosition().x + m_buttonRect.getSize().x &&
			sfmlEvent.mouseMove.y >= getPosition().y && sfmlEvent.mouseMove.y <= getPosition().y + m_buttonRect.getSize().y) {
			m_buttonRect.setFillColor(m_highlightedColor);
		} else {
			m_buttonRect.setFillColor(m_defaultColor);
		} break;
	}
}


bool Button::isClicked() {
	if (m_isClicked) {
		m_isClicked = false;
		m_buttonRect.setFillColor(m_defaultColor);
		return true;
	} else return false;
}


void SliderFloat::updateValue(float delta) {
	m_slideBarRect.setPosition(delta, m_slideBarRect.getPosition().y);
	m_value = m_slideBarRect.getPosition().x * (m_max - m_min) / m_sliderRect.getSize().x + m_min;
}


void SliderInt::updateValue(float delta) {
	int steps = std::abs(m_max - m_min);
	int t = static_cast<int>(std::roundf(steps * delta / m_sliderRect.getSize().x));
	m_slideBarRect.setPosition(m_sliderRect.getSize().x * t / steps, m_slideBarRect.getPosition().y);
	m_value = t + m_min;
}
