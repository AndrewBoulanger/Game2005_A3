#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"

// #define METERS_PER_PIXEL 1.0f/100.0f

class Bullet final : public DisplayObject {
public:
	Bullet();
	~Bullet();

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


	float getGravity();
	bool IsActive();

	void setPixelsPerMeter(float PPM);

	void setFriction(float val);
	void setGravity(float val);
	void toggleActive();
	void setActive(bool flag);
	glm::vec2 getNetForce();


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


#endif /* defined (__BULLET__) */