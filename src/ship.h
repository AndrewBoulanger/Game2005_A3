#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "DisplayObject.h"
#include "TextureManager.h"

class Ship final : public DisplayObject
{
public:
	Ship();
	~Ship();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void stopMovingX();
	void stopMovingY();

	void move();

	// getters
	glm::vec2 getDirection() const;

	// setters
	void setDirection(glm::vec2 newDirection);
	void SetMaxSpeed(float speed);
	void SetAcceleration(float val);

private:
	void m_checkBounds();
	void m_reset();


	glm::vec2 m_direction;
	float m_angle;
	float m_accelerationRate;
	float m_maxSpeed;

};


#endif /* defined (__SHIP__) */
