#pragma once
#ifndef __BULLETPOOL__
#define __BULLETPOOL__

#include "Bullet.h"
#include <deque>
#include <iostream>

using namespace std;

class BulletPool
{
public:
	//constructors
	BulletPool();
	BulletPool(int size);
	~BulletPool();

	//functions
	void resize(int newSize);

	Bullet* getNextInactiveBullet();
	void deactivateBullet();
	bool hasAvailableBullets();

	int getSize();
	int getNumberOfActive();

	deque<Bullet*>* const getdeque() ;

private:
	deque<Bullet*> m_pBullets;
	int m_nextActive, m_size;

	//private functions
	void fillPool();
	void drainPool();
};

#endif /* defined (__BULLETPOOL__) */