#include "Polygon.h"
#include "Renderer.h"
#include "Util.h"
#include <iostream>

Polygon::Polygon()
{
	m_diameter = 40.0f;
	setWidth(m_diameter);
	setHeight(m_diameter);
	m_sides = 20;
	m_active = false;
	m_initialVelocity = glm::vec2(5.0f, -1.0f);
	getTransform()->position = glm::vec2(150.0f, 350.0f);
	getRigidBody()->velocity = m_initialVelocity;
	getRigidBody()->mass = 2;
	m_elasticity = 0.95;
	getRigidBody()->isColliding = false;
	createShape(m_sides);
}

Polygon::~Polygon()
= default;

void Polygon::draw()
{
	SDL_Renderer* renderer = Renderer::Instance()->getRenderer();
	SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
	for (int i = 0; i < m_sides -1; i++)
	{
		SDL_RenderDrawLineF(renderer, m_vertices[i].x, m_vertices[i].y, m_vertices[i + 1].x, m_vertices[i + 1].y);
	}
	SDL_RenderDrawLineF(renderer, m_vertices[m_sides - 1].x, m_vertices[m_sides - 1].y, m_vertices[0].x, m_vertices[0].y);

}

void Polygon::update()
{
	if (m_active)
	{
		UpdateVerticesPosition(getRigidBody()->velocity);
		m_checkBounds();
	}
}

void Polygon::clean()
{
}

void Polygon::createShape(int n)
{
	m_sides = (n < 3) ? 3 : n;  //needs to have at least 3 sides

	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	const float r = m_diameter * 0.5f;
	int lowest = 0;
	int highest = 0;
	int leftest = 0;
	float theta = 0.0f;

	for (int i = 0; i < n; i++)
	{
		m_vertices[i] = glm::vec2(x + r * cos(theta), y + r * sin(theta));
		theta += 2 * PI / n;

		lowest = (m_vertices[i].y < m_vertices[lowest].y) ? i : lowest;
		highest = (m_vertices[i].y > m_vertices[highest].y) ? i : highest;
		leftest = (m_vertices[i].x < m_vertices[leftest].x) ? i : leftest;
	}
	setWidth(m_vertices[0].x - m_vertices[leftest].x);
	setHeight(m_vertices[highest].y - m_vertices[lowest].y);
}

void Polygon::reset()
{
	getTransform()->position = glm::vec2(150.0f, 350.0f);
	createShape(m_sides);
	getRigidBody()->velocity = m_initialVelocity;
	m_active = false;
}

bool Polygon::IsActive()
{
	return m_active;
}

void Polygon::setActive(bool flag)
{
	m_active = flag;
	getRigidBody()->velocity = m_initialVelocity;
}

bool Polygon::m_checkCollision(GameObject* otherObject)
{
	// Checking Polygon Collision with a Rectangular object
	glm::vec2 start;
	glm::vec2 end;
	glm::vec2 BallPos = getTransform()->position;

	// Collision Info
	int brickWidth = otherObject->getWidth();
	int brickHeight = otherObject->getHeight();

	glm::vec2 TopLeft = otherObject->getTransform()->position - glm::vec2(brickWidth * 0.5f, brickHeight * 0.5f);

	for (int i = 0; i < m_sides; i++)
	{
		// if on last side
		if (i + 1 == m_sides)
		{
			// vector for last vertice to first
			start = BallPos + m_vertices[0];
			end = BallPos + m_vertices[i];
		}
		else
		{
			// else we are creating lines for the shape
			start = BallPos + m_vertices[i + 1];
			end = BallPos + m_vertices[i];
		}

		// Check collision with the line
		if (CollisionManager::lineRectCheck(start, end, TopLeft, brickWidth, brickHeight))
		{
			return true;
		}
	}

	// Went through all sides without returning true
	return false;
}

void Polygon::collisionResponse(GameObject* otherObject)
{
	// Collision Info
	int brickWidth = otherObject->getWidth();
	int brickHeight = otherObject->getHeight();

	glm::vec2 TopLeft = otherObject->getTransform()->position - glm::vec2(brickWidth * 0.5f, brickHeight * 0.5f);
	glm::vec2 BottomRight = otherObject->getTransform()->position + glm::vec2(brickWidth * 0.5f, brickHeight * 0.5f);

	// Check Ball's Position relative to Object, change Ball Accordingly
	glm::vec2 BallPos = getTransform()->position;
	float BallRadius = m_diameter * 0.5f;

	// Hitting Right Side
	if (BallPos.x > BottomRight.x)
	{
		// Change Direction (This should use Momentum)
		if (otherObject->getRigidBody()->velocity.x > 0.0f)									//	If the Brick is moving in this direction	
			getRigidBody()->velocity.x = 2 * otherObject->getRigidBody()->velocity.x;		//	Since brick's mass >>> ball's mass, ball's velocity = 2 * brick's velocity
		else																				//	Else the Brick is not moving
		{																					//	
			getRigidBody()->velocity.x *= -1;												//	Acts same as wall, reverses this direction
		}																					//	

		// Move out of Brick
		float distIntoWall = (BottomRight.x - (getTransform()->position.x - BallRadius));
		glm::vec2 WallVec = glm::vec2(-1.0f, 0.0f);

		moveOutOf(WallVec, distIntoWall);
	}

	// Hitting Left Side
	if (BallPos.x < TopLeft.x)
	{
		// Change Direction
		if (otherObject->getRigidBody()->velocity.x < 0.0f)
			getRigidBody()->velocity.x = 2 * otherObject->getRigidBody()->velocity.x;
		else
		{
			getRigidBody()->velocity.x *= -1;
		}

		// Move out of Brick
		float distIntoWall = ((getTransform()->position.x + BallRadius) - TopLeft.x);
		glm::vec2 WallVec = glm::vec2(1.0f, 0.0f);

		moveOutOf(WallVec, distIntoWall);
	}

	// Hitting Top Side
	if (BallPos.y < TopLeft.y)
	{
		// Change Direction
		if (otherObject->getRigidBody()->velocity.y < 0.0f)
			getRigidBody()->velocity.y = 2 * otherObject->getRigidBody()->velocity.y;
		else
		{
			getRigidBody()->velocity.y *= -1;
		}

		// Move out of Brick
		float distIntoWall = ((getTransform()->position.y + BallRadius) - TopLeft.y);
		glm::vec2 WallVec = glm::vec2(0.0f, 1.0f);

		moveOutOf(WallVec, distIntoWall);
	}

	// Hitting Bottom Side
	if (BallPos.y > BottomRight.y)
	{
		// Change Direction
		if (otherObject->getRigidBody()->velocity.y > 0.0f)
			getRigidBody()->velocity.y = 2 * otherObject->getRigidBody()->velocity.y;
		else
		{
			getRigidBody()->velocity.y *= -1;
		}

		// Move out of Brick
		float distIntoWall = (BottomRight.y - (getTransform()->position.y - BallRadius));
		glm::vec2 WallVec = glm::vec2(0.0f, -1.0f);

		moveOutOf(WallVec, distIntoWall);
	}

	// Energy Lost
	getRigidBody()->velocity *= m_elasticity;
}

/* Wall Dir is the Wall's Normal Vector * -1, dist is the distance travelled into the wall. */
void Polygon::moveOutOf(glm::vec2 WallDir, float dist)
{
	glm::vec2 MoveBackDir = Util::normalize(getRigidBody()->velocity);										//	Direction Ball moved into wall at
	glm::vec2 BackVec = MoveBackDir * (dist * cos(Util::Deg2Rad * Util::angle(WallDir, MoveBackDir)));		//	Vector of Distance into the wall
	getTransform()->position -= BackVec;																	//	Subtracting Distance into wall
}

void Polygon::m_checkBounds()
{
	const float nextX = getRigidBody()->velocity.x;
	const float nextY = getRigidBody()->velocity.y;
	const float angle = CalculateAngleOfMovement();
	float yOffset = getHeight() * 0.5;
	

	if(m_vertices[0].x + nextX > Config::SCREEN_WIDTH)
	{
		float newX = Config::SCREEN_WIDTH - m_vertices[0].x;      //x distance to wall
		float newY = tan(angle) * newX;									//account for y movement
		UpdateVerticesPosition(glm::vec2(newX, newY));					//move all vertices 
		getRigidBody()->velocity.x *= -1;								//reverse x direction
		getRigidBody()->velocity *= m_elasticity;
	}
	else if (m_vertices[0].x - getWidth() + nextX <= 0)  //left
	{
		float newX = getWidth() - m_vertices[0].x;
		float newY = tan(angle) * newX;
		UpdateVerticesPosition(glm::vec2(newX, newY));
		getRigidBody()->velocity.x *= -1;								//reverse x direction
		getRigidBody()->velocity *= m_elasticity;
	}
	if (getTransform()->position.y + yOffset + nextY > Config::SCREEN_HEIGHT)  //bottom
	{
		float newY = Config::SCREEN_HEIGHT - yOffset - getTransform()->position.y;      //y distance to wall
		float newX = (tan(angle) == 0.0f) ? 0.0f : newY / tan(angle);		//account for x movement (dont divide by 0)
		UpdateVerticesPosition(glm::vec2(newX, newY));					//move all vertices 
		getRigidBody()->velocity.y *= -1;								//reverse x direction
		getRigidBody()->velocity *= m_elasticity;
	
	}
	else if (getTransform()->position.y - yOffset + nextY <0 )//top
	{
		float newY = yOffset - getTransform()->position.y;
		float newX = (tan(angle) == 0.0f) ? 0.0f : newY / tan(angle);
		UpdateVerticesPosition(glm::vec2(newX, newY));
		getRigidBody()->velocity.y *= -1;								//reverse x direction
		getRigidBody()->velocity *= m_elasticity;
		
	}

}


void Polygon::setInitialVelocity(glm::vec2 newDir)
{
	m_initialVelocity = newDir;
}

glm::vec2 Polygon::GetMomentum()
{
	getRigidBody()->isColliding;
	return getRigidBody()->mass * getRigidBody()->velocity;
}

void Polygon::setElasticity(float val)
{
	m_elasticity = val;
}

int Polygon::getSides()
{
	return m_sides;
}

float Polygon::CalculateAngleOfMovement()
{
	const float X = getRigidBody()->velocity.x;
	const float Y = getRigidBody()->velocity.y;
	float angle = 0.0f;
	if (X != 0)
		angle = atan2(Y, X);
	else
	{
		angle = (Y > 0) ? Util::Deg2Rad * -90.0f : Util::Deg2Rad * 90.0f;
	}
	return angle;
}

void Polygon::UpdateVerticesPosition(glm::vec2 newPos)
{
	getTransform()->position += newPos;
	for (int i = 0; i < m_sides; i++)
	{
		m_vertices[i] += newPos;
	}
}

int Polygon::verticesBoundsCheck()
{
	const float nextX = getRigidBody()->velocity.x;
	const float nextY = getRigidBody()->velocity.y;
	for (int i = 0; i < m_sides; i++)
	{  //right of screen
		if (m_vertices[i].x + nextX > Config::SCREEN_WIDTH)
			return 0;
		//left 
		else if (m_vertices[i].x + nextX < 0)
			return 1;
		//bottom
		else if (m_vertices[i].y + nextY > Config::SCREEN_HEIGHT)
			return 2;
		//top
		else if (m_vertices[i].y + nextY < 0)
			return 3;
	}
return -1;
}


