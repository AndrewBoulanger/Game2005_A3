#include "BallScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#define PPM 30

BallScene::BallScene()
{
	BallScene::start();
}

BallScene::~BallScene()
= default;

bool BallScene::m_viewForce = false;
bool BallScene::m_viewVelocity = false;

void BallScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x, m_trianglePos.y - m_rise);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x + m_run, m_trianglePos.y);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y - m_rise, m_trianglePos.x + m_run, m_trianglePos.y);

	drawDisplayList();


	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 0);

}

void BallScene::update()
{
	if (m_pBall->getSides() > 6)  //circle/rect check
	{
		if (CollisionManager::circleAABBCheck(m_pBall, m_pBrick))
		{
			m_pBall->collisionResponse(m_pBrick);
		}
	}
	else
	{
		if (m_pBall->m_checkCollision(m_pBrick))
		{
			m_pBall->collisionResponse(m_pBrick);
		}
	}

	updateDisplayList();
}

void BallScene::clean()
{
	removeAllChildren();
}

void BallScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().getMouseWheel() < 0)
	{
		m_pBrick->moveDown();
	}
	else if (EventManager::Instance().getMouseWheel() > 0)
	{
		m_pBrick->moveUp();
	}
	else
	{
		m_pBrick->stopMoving();
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

void BallScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Pixels Per Meter
	m_PPM = PPM;

	m_mouseWheel = EventManager::Instance().getMouseWheel();

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

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 16, { 255,255,255,255 });
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pBall = new Polygon();
	addChild(m_pBall);

	m_pBrick = new Brick();
	addChild(m_pBrick);

	m_maxVelocity = 0;

}

void BallScene::GUI_Function() const
{

	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float mass = 2.0f;
	static glm::vec2 initialVelocity = glm::vec2(5, -1);
	static int sides = 20;
	static float elasticity = 0.95f;

	
	if (ImGui::Button(m_pBall->IsActive() ? "Stop" : "Start"))
	{
		if (m_pBall->IsActive())
		{
			m_pBall->reset();

		}
		else
			m_pBall->setActive(true);

	}

	if (ImGui::Button("Reset To Default"))
	{
		// Reset to Default values
		
		mass = 2.0f;
		m_pBall->getRigidBody()->mass = mass;
		elasticity = 0.95f;
		m_pBall->setElasticity(elasticity);
		sides = 20;
		m_pBall->createShape(sides);
		initialVelocity = glm::vec2(5, -1);
		m_pBall->setInitialVelocity(initialVelocity);
		m_pBall->reset();
	}

	ImGui::Separator();

	if (ImGui::SliderInt("number of sides", &sides, 3, 20)) {
		m_pBall->createShape(sides);
	}


	if (ImGui::SliderFloat("mass of ball (kg)", &mass, 0.01f, 15.0f)) {
		m_pBall->getRigidBody()->mass = mass;
	}

	if (ImGui::SliderFloat2("Initial velocity of ball (m/s)", &initialVelocity.x,  -20.0f, 20.0f)) {
		m_pBall->reset();
		m_pBall->setInitialVelocity(initialVelocity);
	}

	if (ImGui::SliderFloat("energy reflected", &elasticity, 0.0f, 1.0f)) {
		m_pBall->setElasticity(elasticity);
	}


	
	ImGui::Text("Speed:  %0.2fm/s", Util::magnitude(m_pBall->getRigidBody()->velocity));
	ImGui::Text("Momentum of the ball:  %0.2fkg m/s", Util::magnitude(m_pBall->getRigidBody()->velocity) * mass);


	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool BallScene::StartSim()
{
	return false;
}

