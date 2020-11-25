#include "Polygon.h"
#include "Renderer.h"

Polygon::Polygon()
{
	setWidth(40);
	setHeight(40);
	m_sides = 20;
	m_active = false;
	getTransform()->position = glm::vec2(150.0f, 350.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	createShape(m_sides);
}

Polygon::~Polygon()
{
}

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
	const float r = getHeight() * 0.5f;

	float theta = 0.0f;

	for (int i = 0; i < n; i++)
	{
		m_vertices[i] = glm::vec2(x + r * cos(theta), y + r * sin(theta));
		theta += 2 * PI / n;
	}
}

void Polygon::reset()
{
	getTransform()->position = glm::vec2(150.0f, 350.0f);
	m_active = false;
}

bool Polygon::IsActive()
{
	return m_active;
}

void Polygon::setActive(bool flag)
{
	m_active = flag;
}

void Polygon::m_checkCollision(GameObject* otherObject)
{
}


