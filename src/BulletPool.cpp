#include "BulletPool.h"

BulletPool::BulletPool()
{
	m_size = m_nextActive = 0;
}

BulletPool::BulletPool(int size)
{
	m_size = size;
	m_nextActive = 0;
	fillPool();
}

BulletPool::~BulletPool()
{
	drainPool();
}

void BulletPool::resize(int newSize)
{
	drainPool();
	m_size = newSize;
	fillPool();
}

Bullet* BulletPool::getNextInactiveBullet()
{
	if (hasAvailableBullets())
	{
		m_pBullets.at(m_nextActive)->reset();
		return m_pBullets.at(m_nextActive++);
	}
	else
	{
		cout << "No bullets available\n";
		return nullptr;    //remember to check for nullptr when called
	}
}

void BulletPool::deactivateBullet()
{
	if (m_pBullets.front()->IsActive() == false)
	{
		m_pBullets.push_back(m_pBullets.front());
		m_pBullets.pop_front();
		m_nextActive--;
	}
}

//resets bullets and # of active, remember to also remove them as children in the playscene before calling this
void BulletPool::deactivateAll()  
{				
	
	for (int i = 0; i < m_nextActive; i++)
	{
		m_pBullets.at(i)->reset();
	}
	m_nextActive = 0;
}

bool BulletPool::hasAvailableBullets()
{
	return m_nextActive < m_size;
}

int BulletPool::getSize()
{
	return m_size;
}

int BulletPool::getNumberOfActive()
{
	return  m_nextActive ;
}

deque<Bullet*>* const BulletPool::getdeque() 
{
	return &m_pBullets ;
}

//instantiate all objects at once
void BulletPool::fillPool()
{
	for (int i = 0; i < m_size; i++)
	{
		m_pBullets.push_back(new Bullet);
	}
}

//delete and remove all objects in the pool, set size to zero
void BulletPool::drainPool()
{
	while (m_pBullets.empty() == false)
	{
		delete m_pBullets.front();
		m_pBullets.pop_front();
		
	}
	m_size = m_nextActive = 0;
}
