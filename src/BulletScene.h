#pragma once
#ifndef __BULLET_SCENE__
#define __BULLET_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Ship.h"
#include "Button.h"
#include "Label.h"
#include "Bullet.h"
#include "BulletPool.h"
#include <deque>

class BulletScene : public Scene
{
public:
	BulletScene();
	~BulletScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	bool StartSim();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	float m_PPM;

	Ship* m_pShip;


	bool m_sceneActive;

	BulletPool* m_pbulletPool;
	float m_spawnTime, m_timer;

	void SpawnBullet();
	void DeactivateAllBullets();

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_pTempLabel;
	float m_maxVelocity;	//used for speed label
	static bool m_viewForce;
	static bool m_viewVelocity;

};

#endif /* defined (__BULLET_SCENE__) */