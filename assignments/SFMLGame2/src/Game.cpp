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

void Game::init(const std::string &config)
{
	std::ifstream fin(config);
	if (!fin)
	{
		std::cerr << "Failed to open config file: " << config << "." << std::endl;
		exit(-1);
	}

	std::string type;
	fin >> type;
	if (type != "Window")
	{
		std::cerr << "Expected first declaration to be Window, got: " << type << " instead." << std::endl;
		exit(-1);
	}

	int w, h, fps;
	bool fs;
	fin >> w >> h >> fps >> fs;
	m_window.create(sf::VideoMode(w, h), "SFMLGame2", fs ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.setFramerateLimit(fps);

	fin >> type;
	if (type != "Font")
	{
		std::cerr << "Expected second declaration to be Font, got: " << type << " instead." << std::endl;
		exit(-1);
	}

	std::string fontFile;
	int fontSize, fontR, fontG, fontB;
	fin >> fontFile >> fontSize >> fontR >> fontG >> fontB;
	if (!m_font.loadFromFile(fontFile)) {
		std::cerr << "Could not load font: " << fontFile << "." << std::endl;
		exit(-1);
	}

	while (fin >> type)
	{
		if (type == "Player")
		{
			fin >> m_playerConfig.SR >> m_playerConfig.CR
				>> m_playerConfig.S
				>> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
				>> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
				>> m_playerConfig.OT
				>> m_playerConfig.V;
				
			continue;
		}

		if (type == "Enemy")
		{
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR
				>> m_enemyConfig.SMIN >> m_enemyConfig.SMAX
				>> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB
				>> m_enemyConfig.OT
				>> m_enemyConfig.VMIN >> m_enemyConfig.VMAX
				>> m_enemyConfig.L >> m_enemyConfig.SI;
			continue;
		}

		if (type == "Bullet")
		{
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR
				>> m_bulletConfig.S
				>> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
				>> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB
				>> m_bulletConfig.OT
				>> m_bulletConfig.V >> m_bulletConfig.L;
			continue;
		}

		std::cerr << "Invalid config entry: " << type << "." << std::endl;
		fin.ignore('\n');
	}

	std::cout << "Finished reading config..." << std::endl;
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
	m_window.draw(m_player->cShape->circle);

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
	sf::Vector2u size = m_window.getSize();
	entity->cTransform = std::make_shared<CTransform>(Vec2(size.x / 2.0f, size.y / 2.0f), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill , and red outline of tthickness 4
	entity->cShape = std::make_shared<CShape>(
		m_playerConfig.SR, 
		m_playerConfig.V, 
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), 
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

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
