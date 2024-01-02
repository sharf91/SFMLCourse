#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class BasicShape
{
private:
	std::shared_ptr<sf::Text> m_name;
	std::shared_ptr<sf::Shape> m_shape;
	float m_fVelX = 0.0f, m_fVelY = 0.0f;
public:
	BasicShape(std::shared_ptr<sf::Shape> shape, std::shared_ptr<sf::Text> name, float posX, float posY, float velX, float velY);

	float getVelX() const;
	float getVelY() const;

	void reverseXVel();
	void reverseYVel();

	void handleBounds(float left, float top, float w, float h);

	void moveNameToShape();
	void update();

	const std::shared_ptr<sf::Shape> getShape() const;
	const std::shared_ptr<sf::Text> getName() const;
};

