#include "Box.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"

#define DELTA_TIME 1.0f / 60.0f
Box::Box()
{
	TextureManager::Instance()->load("../Assets/textures/lootbox.png","lootbox");

	getTransform()->position = glm::vec2(200.0f, 150.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	getRigidBody()->mass = 12.8;
	m_gravity = 9.8f ;

	m_active = false;


	m_friction = 0.42f ;

	setDiretion(glm::vec2(0.8f, 0.6f));
	m_angle = 0;
}

Box::~Box()
= default;

void Box::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("lootbox", x, y - (getHeight()*.75), getWidth(), getHeight(), m_angle, 255, false);
}

void Box::update()
{
	if(m_active)
		m_move();
}

void Box::clean()
{
}

void Box::m_move()
{
	glm::vec2 accelToAdd = getRigidBody()->acceleration * DELTA_TIME;
	getRigidBody()->velocity += accelToAdd;

	if (Util::magnitude(getRigidBody()->velocity) < Util::magnitude(accelToAdd))
	{
		m_active = false;	
		getTransform()->position += getRigidBody()->velocity - accelToAdd ;
	}
	
	// Update Position here
	getTransform()->position += getRigidBody()->velocity * DELTA_TIME;

}

void Box::m_checkBounds()
{
}

void Box::m_reset()
{
}


void Box::reset(float x, float y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;

	m_angle = 0;
	m_active = false;
	getRigidBody()->acceleration *= 0;
	getRigidBody()->velocity *= 0;
}

glm::vec2 Box::getDirection()
{
	return m_direction;
}

float Box::getFriction()
{
	return m_friction;
}

float Box::getGravity()
{
	return m_gravity;
}

bool Box::IsActive()
{
	return m_active;
}

void Box::setPixelsPerMeter(float PPM)
{
	m_PPM = PPM;
}

//takes the rise and run of the slope
void Box::setDiretion(glm::vec2 dir)
{
	m_direction = dir;

	m_angle = glm::degrees(glm::atan(dir.y, dir.x));

	getRigidBody()->acceleration = dir * glm::sin(glm::radians(m_angle)) * -m_gravity;
	if (getRigidBody()->acceleration.x < 0)
		getRigidBody()->acceleration *= -1;
}


void Box::setFriction(float val)
{
	m_friction = val;
}

void Box::setGravity(float val)
{
	m_gravity = val * m_PPM;
}

void Box::toggleActive()
{
	m_active = !m_active;
}

void Box::setActive(bool flag)
{
	m_active = flag;
}

glm::vec2 Box::getNetForce()
{
	return glm::vec2(getRigidBody()->acceleration.x * getRigidBody()->mass, getRigidBody()->acceleration.y * getRigidBody()->mass);
}
