#pragma once
#ifndef __BOX__
#define __BOX__

#include "DisplayObject.h"

// #define METERS_PER_PIXEL 1.0f/100.0f

class Box final : public DisplayObject {
public:
	Box();
	~Box();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	float GetAngle()
	{
		return m_angle;
	}
	void SetAngle(float angle)
	{
		m_angle = angle;
	}

	void reset(float x, float y);

	glm::vec2 getDirection();
	float getFriction();
	float getGravity();
	bool IsActive();

	void setPixelsPerMeter(float PPM);
	void setDiretion(glm::vec2 dir);
	void setFriction(float val);
	void setGravity(float val);
	void toggleActive();
	void setActive(bool flag);
	glm::vec2 getNetForce();

	float GetMaxVelocity();

private:
	void m_move();
	void m_checkBounds();
	void m_reset();

	float m_angle;

	//movement variables
	glm::vec2 m_direction;
	float m_friction, m_gravity, m_PPM;

	bool m_active;
};


#endif /* defined (__BOX__) */