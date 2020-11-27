#pragma once
#include "DisplayObject.h"

class Brick final : public DisplayObject
{
public:
	Brick();
	~Brick();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void moveUp();
	void moveDown();
	void stopMoving();

	void checkBounds();
private:
	float m_accelerationRate;
	float m_directionY;

};

