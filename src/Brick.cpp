#include "Brick.h"
#include "glm/gtx/string_cast.hpp"
#include "BulletScene.h"
#include "TextureManager.h"
#include "Util.h"

Brick::Brick()
{
	TextureManager::Instance()->load("../Assets/textures/brick.png", "brick");

	setWidth(30.0f);
	setHeight(80.0f);

	getTransform()->position = glm::vec2(400.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	m_directionY = 0;
	getRigidBody()->mass = 5;
	getRigidBody()->isColliding = false;
	m_accelerationRate = 10;

}

Brick::~Brick()
= default;

void Brick::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("brick", x, y, getWidth(), getHeight(), 0, 255, true);
}

void Brick::update()
{
	const float deltaTime = 1.0f / 60.f;

	//set acceleration (I should make this its own function at some point)
	if (m_directionY != 0)
	{
		//Speed up: direction key pressed, set acceleration to acceleration factor * unit direction
		getRigidBody()->acceleration.y =  m_directionY * m_accelerationRate;
	}
	else if (Util::magnitude(getRigidBody()->velocity) < m_accelerationRate)
	{
		//Stop: no input and velocity is negligable, so acceleration and velocity should be nothing
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	}
	else
	{
		//Slow down: no keys pressed, but player has velocity, set acceleration to unit direction (from current velocity) * -acceleration factor
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -m_accelerationRate;
	}

	getRigidBody()->velocity += getRigidBody()->acceleration;
	glm::vec2 pos = getTransform()->position;
	
	pos.y += getRigidBody()->velocity.y ;

	getTransform()->position.y =  pos.y;
	checkBounds();

}

void Brick::clean()
{
}

void Brick::moveUp()
{
	m_directionY = -1;
}

void Brick::moveDown()
{
	m_directionY = 1;
}

void Brick::stopMoving()
{
	m_directionY = 0;
}

void Brick::checkBounds()
{
	if (getTransform()->position.y  > Config::SCREEN_HEIGHT + (getWidth() * 0.5))
	{
		getTransform()->position.y =  -(getWidth() * 0.5);
	}
	else if (getTransform()->position.y <  -(getWidth() * 0.5))
	{
		getTransform()->position.y = Config::SCREEN_HEIGHT + (getWidth() * 0.5);
	}
}
