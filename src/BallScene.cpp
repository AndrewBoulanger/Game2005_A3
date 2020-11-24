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

}

void BallScene::clean()
{
	removeAllChildren();
}

void BallScene::handleEvents()
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

		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{

		}
		else
		{

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

void BallScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Pixels Per Meter
	m_PPM = PPM;


	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "back", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(65.0f, 550.0f);
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
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
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
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20, { 255,255,255,255 });
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pTempLabel = new Label("X from Bot Ramp: ", "Consolas");
	m_pTempLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 20.0f);
	addChild(m_pTempLabel);

	m_maxVelocity = 0;
}

void BallScene::GUI_Function() const
{

	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float height = 3.0f;
	static float length = 4.0f;
	static float CoefficientFriction = 0.42f;
	static float mass = 12.8f;

	if (ImGui::Button("Reset To Default"))
	{
		// Reset to Default values
		height = 3.0f;
		length = 4.0f;
		CoefficientFriction = 0.42f;
		mass = 12.8f;

	}

	ImGui::Separator();


	if (ImGui::SliderFloat("Height (m)", &height, 0.01f, 15.0f)) {
		(float)m_rise = height * m_PPM;
	}

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Angle of depression:  %f degrees", -(glm::degrees(glm::atan(m_rise, m_run))));

	if (ImGui::SliderFloat("Length (m)", &length, 0.01f, 20.0f)) {
		(float)m_run = length * m_PPM;
	}

	ImGui::SameLine(350.0F, 1);


	if (ImGui::SliderFloat("Co. of Friction", &CoefficientFriction, 0.0f, 3.0f)) {
	}


	if (ImGui::SliderFloat("Mass (kg)", &mass, 0.1f, 200.0f)) {

	}


	ImGui::Checkbox("Show Net Force", &m_viewForce);


	ImGui::Checkbox("Show Velocity", &m_viewVelocity);

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Max Speed:            %fm/s", m_maxVelocity);


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

