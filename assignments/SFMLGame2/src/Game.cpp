#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>

#include "Game.h"
#include "Vec2.h"

Game::Game(const std::string& config)
	: m_score(0)
	, m_currentFrame(0)
	, m_lastEnemySpawnTime(0)
	, m_paused(false)
	, m_running(true) 
{
	init(config);
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		m_currentFrame++;
	}
}

void Game::init(const std::string& config)
{

	m_window.create(sf::VideoMode(1280, 720), "SFMLGame2");;
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::sMovement()
{
	if (m_player->cInput->up)
	{
		m_player->cTransform->vel.y = -5;
	}
	else
	{
		m_player->cTransform->vel.y = 0;
	}

	m_player->cTransform->pos.x += m_player->cTransform->vel.x;
	m_player->cTransform->pos.y += m_player->cTransform->vel.y;
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W+" << std::endl;
				m_player->cInput->up = true;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W-" << std::endl;
				m_player->cInput->up = false;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "ML+ at " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "MR+ at " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
			}
		}
	}
}

void Game::sLifeSpan()
{
}

void Game::sRender()
{
	m_window.clear();

	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	m_window.display();
}

void Game::sEnemySpawner()
{
	spawnEnemy();
}

void Game::sCollision()
{
}

void Game::spawnPlayer()
{
	// TODO: Finish adding all properties of the player with the correct values from the config

	// We create every entity by calling EntityManagger.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use auto to save typing
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spawns at (200, 200) with the velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill , and red outline of tthickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our playerm set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
	m_player = entity;
}

void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	entity->cTransform = std::make_shared<CTransform>(Vec2(500.0f, 500.0f), Vec2(1.0f, 1.0f), 0.0f);
	entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.0f);
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmaLLEnemies(std::shared_ptr<Entity> entity)
{
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
