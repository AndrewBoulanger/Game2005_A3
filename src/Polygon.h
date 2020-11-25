#pragma once
#ifndef __POLYGON__
#define __POLYGON__

#include "DisplayObject.h"
#include <array>

#define MAXVERTICES 20
#define PI 3.14159265358979324

class Polygon final: public DisplayObject
{
public:
    Polygon();
    ~Polygon();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void createShape(int n);

	void reset();

	bool IsActive();
	void setActive(bool flag);
	
	void m_checkCollision(GameObject* otherObject);

	void m_move();
	void m_checkBounds();

	void setSides(int sides);

private:
	float m_angle;
	int m_sides;
	std::array<glm::vec2, MAXVERTICES> m_vertices;

	//movement variables
	glm::vec2 m_direction;
	float m_mass;

	bool m_active;
	bool m_overlapping;
};


#endif  /*defined (__POLYGON__) */