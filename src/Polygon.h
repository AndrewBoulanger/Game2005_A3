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
	
	bool m_checkCollision(GameObject* otherObject);
	void collisionResponse(GameObject* otherObject);
	void moveOutOf(glm::vec2 WallDir, float dist);

	void m_checkBounds();

	/*no setMass() - mass handled by rigidbody*/
	void setInitialVelocity(glm::vec2 newDir);
	glm::vec2 GetMomentum();
	void setElasticity(float val);
	int getSides();
	
private:
	float m_diameter;
	float m_angle;
	int m_sides;
	std::array<glm::vec2, MAXVERTICES> m_vertices;

	//movement variables
	glm::vec2 m_initialVelocity;
	glm::vec2 m_momentum;
	float m_elasticity;

	bool m_active;
	bool m_overlapping;

	float CalculateAngleOfMovement();
	void UpdateVerticesPosition(glm::vec2 newPos);
	int verticesBoundsCheck();

};


#endif  /*defined (__POLYGON__) */