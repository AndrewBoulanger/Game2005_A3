#include "Bullet.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"


#define DELTA_TIME 1.0f / 60.0f
Bullet::Bullet()
{
	TextureManager::Instance()->load("../Assets/textures/Asteroid.png","Asteroid");
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "collision", SOUND_SFX);
	setWidth(50);
	setHeight(45);

	getTransform()->position = glm::vec2(rand()%(Config::SCREEN_WIDTH - getWidth()), -getHeight());
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	getRigidBody()->mass = 12.8;
	m_gravity = Game::Instance()->getGravity() ;

	m_active = false;


	m_friction = 0.42f ;

	m_direction = glm::vec2(0.0f, 1.0f);

	m_angle = 0;
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("Asteroid", x, y, getWidth(), getHeight(), m_angle, 255, false);

}

void Bullet::update()
{
	if (m_active)
	{
		m_move();
		m_checkBounds();
		m_angle++;
	}
}

void Bullet::clean()
{
}

void Bullet::m_move()
{
	getRigidBody()->acceleration = { 0, m_gravity };
	glm::vec2 accelToAdd = getRigidBody()->acceleration * DELTA_TIME;
	getRigidBody()->velocity += accelToAdd;

	// Update Position here
	getTransform()->position += getRigidBody()->velocity * DELTA_TIME;
	
}

void Bullet::m_checkBounds()
{
	if (getTransform()->position.y > 600)
	{
		m_active = false;
		reset();
	}
}

void Bullet::m_checkCollision(GameObject* otherObject)
{
	if (CollisionManager::squaredRadiusCheck(this, otherObject, 0.85f, 0.85f))
	{
		if (m_overlapping == false)
		{
			m_overlapping = true;
			SoundManager::Instance().playSound("collision");
		}
	}
	else 
		m_overlapping = false;
}

void Bullet::m_reset()
{
}


void Bullet::reset()
{
	getTransform()->position.x = rand() % (Config::SCREEN_WIDTH - getWidth());
	getTransform()->position.y = -getHeight();

	m_angle = 0;
	m_active = false;
	getRigidBody()->acceleration = { 0.0F,0.0F };
	getRigidBody()->velocity = { 0.0F,0.0F };

	m_gravity = Game::Instance()->getGravity();
}


float Bullet::getGravity()
{
	return m_gravity;
}

bool Bullet::IsActive()
{
	return m_active;
}

void Bullet::setPixelsPerMeter(float PPM)
{
	m_PPM = PPM;
}



void Bullet::setFriction(float val)
{
	m_friction = val;
}

void Bullet::setGravity(float val)
{
	m_gravity = val * m_PPM;
}

void Bullet::toggleActive()
{
	m_active = !m_active;
}

void Bullet::setActive(bool flag)
{
	m_active = flag;
}

glm::vec2 Bullet::getNetForce()
{
	return glm::vec2(getRigidBody()->acceleration.x * getRigidBody()->mass, getRigidBody()->acceleration.y * getRigidBody()->mass);
}
