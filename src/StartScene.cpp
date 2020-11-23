#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(BULLET_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");

	const SDL_Color orange = { 255, 155, 0, 255 };
	m_pStartLabel = new Label("GAME2005 Assignment 3 ", "Consolas", 60, orange, glm::vec2(Config::SCREEN_WIDTH >> 1, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pNameLabel1 = new Label("Andrew Boulanger - 101 292 574", "Consolas", 32, orange, glm::vec2(Config::SCREEN_WIDTH >> 1, 110.0f));
	m_pNameLabel1->setParent(this);
	addChild(m_pNameLabel1);

	m_pNameLabel2 = new Label("Robert Palermo - 101 277 078", "Consolas", 32, orange, glm::vec2(Config::SCREEN_WIDTH >> 1, 170.0f));
	m_pNameLabel2->setParent(this);
	addChild(m_pNameLabel2);

	m_pInstructionsLabel = new Label("press start button to Play", "Consolas", 40, orange, glm::vec2(Config::SCREEN_WIDTH >> 1, 500.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);


	// Start Button
	m_pBulletLevelButton = new Button("../Assets/textures/BulletSceneButton.png", "bulletlevel");
	m_pBulletLevelButton->getTransform()->position = glm::vec2(250.0f, 400.0f); 

	m_pBulletLevelButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBulletLevelButton->setActive(false);
		TheGame::Instance()->changeSceneState(BULLET_SCENE);
	});
	
	m_pBulletLevelButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBulletLevelButton->setAlpha((Uint8)128);
	});

	m_pBulletLevelButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBulletLevelButton->setAlpha(255);
	});
	addChild(m_pBulletLevelButton);

	// Start Button
	m_pBallLevelButton = new Button("../Assets/textures/BallSceneButton.png", "BalllLevel");
	m_pBallLevelButton->getTransform()->position = glm::vec2(500.0f, 400.0f);


	m_pBallLevelButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBallLevelButton->setActive(false);
		TheGame::Instance()->changeSceneState(BALL_SCENE);
	});

	m_pBallLevelButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBallLevelButton->setAlpha(128);
		
	});

	m_pBallLevelButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBallLevelButton->setAlpha(255);
	});
	addChild(m_pBallLevelButton);

	
}

