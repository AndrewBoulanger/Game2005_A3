#include "ship.h"
#include "glm/gtx/string_cast.hpp"
#include "BulletScene.h"
#include "TextureManager.h"
#include "Util.h"


Ship::Ship()
{
	TextureManager::Instance()->load("../Assets/textures/ship3.png", "ship");

	auto size = TextureManager::Instance()->getTextureSize("ship");
	setWidth(50);
	setHeight(40);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);

	m_direction = glm::vec2(1.0f, 0.0f); // facing right
	m_angle = 0;

	m_accelerationRate = 8;
	m_maxSpeed = 300;
}


Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("ship", x, y, getWidth(), getHeight(), m_angle, 255);
}


void Ship::update()
{
	move();
	m_checkBounds();
}

void Ship::clean()
{
}

void Ship::moveRight()
{
	m_direction.x = 1;
	m_angle = 90;
}

void Ship::moveLeft()
{
	m_direction.x = -1;
	m_angle = -90;
}

void Ship::moveUp()
{
	m_direction.y = -1;
	m_angle = 0;
}

void Ship::moveDown()
{
	m_direction.y = 1;
	m_angle = 180;
}

void Ship::stopMovingX()
{
	m_direction.x = 0;
}

void Ship::stopMovingY()
{
	m_direction.y = 0;
}

void Ship::move()
{
	const float deltaTime = 1.0f / 60.f;

	float dirMagnitude = Util::magnitude(m_direction);
	//set acceleration (I should make this its own function at some point)
	if (dirMagnitude > 0)
	{
		//Speed up: direction key pressed, set acceleration to acceleration factor * unit direction
		getRigidBody()->acceleration = Util::normalize(m_direction) * m_accelerationRate;
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
	getRigidBody()->velocity = Util::limitMagnitude(getRigidBody()->velocity, m_maxSpeed);


	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;


	getTransform()->position = pos;
}


glm::vec2 Ship::getDirection() const
{
	return m_direction;
}



void Ship::setDirection(glm::vec2 newDirection)
{
	m_direction = newDirection;
}

void Ship::SetMaxSpeed(float speed)
{
	m_maxSpeed = speed;
}

void Ship::SetAcceleration(float val)
{
	m_accelerationRate = val;
}



void Ship::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Ship::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}


