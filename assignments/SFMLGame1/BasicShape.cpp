#include "BasicShape.h"
#include <iostream>

BasicShape::BasicShape(std::shared_ptr<sf::Shape> shape, std::shared_ptr<sf::Text> name, float posX, float posY, float velX, float velY)
    : m_shape(shape)
    , m_name(name)
    , m_fVelX(velX)
    , m_fVelY(velY)
{
    m_shape->setPosition(sf::Vector2f(posX, posY));
    moveNameToShape();
}

float BasicShape::getVelX() const
{
    return m_fVelX;
}

float BasicShape::getVelY() const
{
    return m_fVelY;
}

void BasicShape::reverseXVel()
{
    m_fVelX *= -1.0f;
}

void BasicShape::reverseYVel()
{
    m_fVelY *= -1.0f;
}

void BasicShape::handleBounds(float left, float top, float w, float h)
{
    sf::FloatRect bounds = m_shape->getGlobalBounds();
    if (bounds.left <= 0 || (bounds.left + bounds.width) >= w)
    {
        reverseXVel();

        if (bounds.left <= 0)
        {
            bounds.left = 0;
            m_shape->setPosition(sf::Vector2f(bounds.left, bounds.top));
        }
        else
        {
            bounds.left = left + w - bounds.width;
            m_shape->setPosition(sf::Vector2f(bounds.left, bounds.top));
        }
    }

    if (bounds.top <= 0 || (bounds.top + bounds.height) >= h)
    {
        reverseYVel();

        if (bounds.top <= 0)
        {
            bounds.top = 0;
            m_shape->setPosition(sf::Vector2f(bounds.left, bounds.top));
        }
        else
        {
            bounds.top = top + h - bounds.height;
            m_shape->setPosition(sf::Vector2f(bounds.left, bounds.top));
        }
    }

    moveNameToShape();
}

void BasicShape::moveNameToShape()
{
    sf::FloatRect shapeBounds = m_shape->getGlobalBounds();
    sf::FloatRect textBounds = m_name->getGlobalBounds();

    float centerX = shapeBounds.left + shapeBounds.width / 2 - textBounds.width/2;
    float centerY = shapeBounds.top + shapeBounds.height / 2 - textBounds.height/2;
    m_name->setPosition(sf::Vector2f(centerX, centerY));
}

void BasicShape::update()
{
    m_shape->setPosition(m_shape->getPosition().x + m_fVelX, m_shape->getPosition().y + m_fVelY);
    moveNameToShape();
}

const std::shared_ptr<sf::Shape> BasicShape::getShape() const
{
    return m_shape;
}

const std::shared_ptr<sf::Text> BasicShape::getName() const
{
    return m_name;
}
