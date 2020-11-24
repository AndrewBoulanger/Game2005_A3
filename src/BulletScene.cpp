#include "BulletScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#define PPM 30
#define DELTATIME 1/60

BulletScene::BulletScene()
{
	BulletScene::start();
}

BulletScene::~BulletScene()
= default;

bool BulletScene::m_viewForce = false;
bool BulletScene::m_viewVelocity = false;

void BulletScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(),0,0,0,0);
	
}

void BulletScene::update()
{
	if (m_sceneActive)
	{
		if (m_changeInAsteroidAmount)
		{
			ChangeNumberOfAsteroids();
		}

		m_timer -= 1.0 * DELTATIME;

		if (m_timer <= 0)
		{
			m_timer = m_spawnTime;
			SpawnBullet();
		}

		int numInctive = 0;

		//check active bullets
		for (int i = 0; i < m_pbulletPool->getNumberOfActive(); i++)
		{
			m_pbulletPool->getdeque()->at(i)->m_checkCollision(m_pShip);
			if (m_pbulletPool->getdeque()->at(i)->IsActive() == false)
			{
				numInctive++;  //deactivate bullet will push these off the active side of the deque
				removeChild(m_pbulletPool->getdeque()->at(i), false);
			}
		}

		for (int i = 0; i < numInctive; i++)
			m_pbulletPool->deactivateBullet();
		m_readyToEndSim = true;
	}
	else
		if (m_readyToEndSim)
		{
			DeactivateAllBullets();
			m_readyToEndSim = false;
		}
	updateDisplayList();

}

void BulletScene::clean()
{
	removeAllChildren();
}

void BulletScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
			
			}
			else
			{
			
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pShip->moveLeft();
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pShip->moveRight();
		}
		else
		{
			m_pShip->stopMovingX();
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			m_pShip->moveUp();
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			m_pShip->moveDown();
		}
		else
		{
			m_pShip->stopMovingY();
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void BulletScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Bullet Scene";
	
	// Pixels Per Meter
	m_PPM = PPM;

	m_sceneActive = false;
	m_spawnTime = 3.0f;
	m_timer = 0;
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "back", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(65.0f, 550.0f);
	m_pBackButton->setWidth(100);
	m_pBackButton->setHeight(35);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/startButton.png", "activate", START_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(400.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		if (m_sceneActive)
		{
			m_sceneActive = false;
			DeactivateAllBullets();
		}
		else
			StartSim();
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 16, {255,255,255,255});
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pShip = new Ship();
	addChild(m_pShip);

	//bullet pool
	m_pbulletPool = new BulletPool(10);

}

void BulletScene::GUI_Function() const
{
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Collision simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float gravity = 9.86f;
	static int maxNumberofAsteroids = 10;
	static float SpawnRate = 3.0f;
	static float ShipMaxSpeed = 300.0f;
	static float ShipAcceleration = 8.0f;

	if (ImGui::Button("Reset To Default"))
	{
		// Reset to Default values
		gravity = 9.86f;
		//maxNumberofAsteroids = 10; this feels like too heavy an operation to let it be called over an over again
		SpawnRate = 3.0f;
		ShipMaxSpeed = 300;
		ShipAcceleration = 8.0f;
		(bool)m_sceneActive = false;
		
	}
	if (ImGui::Button(m_sceneActive ? "Stop" : "Start"))
	{
		if (m_sceneActive)
		{
			(bool)m_sceneActive = false;
			
		}
		else
			(bool)m_sceneActive = true;

	}

	ImGui::Separator();
	if (ImGui::SliderInt("Max Number of Asteroids", &maxNumberofAsteroids, 1, 20)) {
		(bool)m_changeInAsteroidAmount = true;
		(bool)m_sceneActive = false;
		(int)m_maxNumberOfAsteroids = maxNumberofAsteroids;
	}
	if (ImGui::SliderFloat("Gravity (m/s^2)", &gravity, 0.01f, 100.0f)) {
		Game::Instance()->setGravity(gravity);
		(bool)m_sceneActive = false;
	}
	if (ImGui::SliderFloat("Asteroid Spawn Rate (s)", &SpawnRate, 0.01f, 10.0f)) {
		(float)m_spawnTime = SpawnRate;
		(bool)m_sceneActive = false;
	}

	ImGui::Separator();


	if (ImGui::SliderFloat("Ship Max Speed (m/s)", &ShipMaxSpeed, 1.0f, 600.0f)) {
		m_pShip->SetMaxSpeed(ShipMaxSpeed);
	}
	if (ImGui::SliderFloat("Ship Acceleration Rate (m/s^2)", &ShipAcceleration, 1.0f, 20.0f)) {
		m_pShip->SetAcceleration(ShipAcceleration);
	}

	ImGui::Separator();
	ImGui::Text("Active Asteroids:  %i", m_pbulletPool->getNumberOfActive());
	ImGui::Text("Current Ship Speed:  %0.2fm/s", Util::magnitude(m_pShip->getRigidBody()->velocity));
	ImGui::Text("Ship Position: X: %0.1f,  Y: %0.1f", m_pShip->getTransform()->position.x, Config::SCREEN_HEIGHT - m_pShip->getTransform()->position.y);

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool BulletScene::StartSim()
{
	m_sceneActive = true;

	return false;
}

void BulletScene::SpawnBullet()
{
	if (m_pbulletPool->hasAvailableBullets())
	{
		Bullet* temp = m_pbulletPool->getNextInactiveBullet();
		addChild(temp);
		temp->setActive(true);
	}
}

void BulletScene::DeactivateAllBullets()
{
	for (int i = 0; i < m_pbulletPool->getNumberOfActive(); i++)
	{
		removeChild(m_pbulletPool->getdeque()->at(i), false);
	}
	m_pbulletPool->deactivateAll();
}

void BulletScene::ChangeNumberOfAsteroids()
{
	m_changeInAsteroidAmount = false;
	DeactivateAllBullets();
	m_pbulletPool->resize(m_maxNumberOfAsteroids);
}


