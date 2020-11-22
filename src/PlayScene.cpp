#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#define PPM 30

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

bool PlayScene::m_viewForce = false;
bool PlayScene::m_viewVelocity = false;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x, m_trianglePos.y - m_rise);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x + m_run, m_trianglePos.y);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y - m_rise, m_trianglePos.x + m_run, m_trianglePos.y);

	drawDisplayList();

	// Drawing Force and Veloctiy Arrows
	if (m_viewForce)
	{
		glm::vec2 Offset = glm::vec2(m_pLootbox->getWidth() / 2, -m_pLootbox->getHeight() / 4);
		glm::vec2 ForceDir = (Util::magnitude(m_pLootbox->getNetForce()) > 0 ? m_pLootbox->getNetForce() : glm::vec2(0.0f, 0.0f));
		glm::vec4 Red = glm::vec4((1.0f), (0.0f), (0.0f), (1.0f));

		DrawArrow(m_pLootbox->getTransform()->position + Offset, ForceDir, m_pLootbox->getRigidBody()->mass / 1000.0f, Red);
	}

	if (m_viewVelocity)
	{
		glm::vec2 Offset = glm::vec2(m_pLootbox->getWidth() / 2, -m_pLootbox->getHeight() / 4);
		glm::vec4 Blue = glm::vec4((0.0f), (0.0f), (1.0f), (1.0f));

		DrawArrow(m_pLootbox->getTransform()->position + Offset, m_pLootbox->getRigidBody()->velocity, Util::magnitude(m_pLootbox->getRigidBody()->velocity / 200.0f), Blue);
	}

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(),0,0,0,0);
	
}

void PlayScene::update()
{
	if (m_pLootbox->getTransform()->position.x >= m_trianglePos.x + m_run)
	{
		m_pLootbox->getRigidBody()->velocity.x = Util::magnitude(m_pLootbox->getRigidBody()->velocity);
		m_pLootbox->getRigidBody()->velocity.y = 0;
		m_pLootbox->getRigidBody()->acceleration.y = 0;
		if (m_pLootbox->getTransform()->position.y > m_trianglePos.y)
		{
			m_pLootbox->getTransform()->position.x -= m_pLootbox->getTransform()->position.y - m_trianglePos.y;
			m_pLootbox->getTransform()->position.y = m_trianglePos.y;

		}
		m_pLootbox->getRigidBody()->acceleration.x = -(m_pLootbox->getFriction() * m_pLootbox->getGravity());
		m_pLootbox->SetAngle(0.0f);
	}
	updateDisplayList();

	float m_deltaXbot = -(m_trianglePos.x + m_run - m_pLootbox->getTransform()->position.x) / m_PPM;

	m_pTempLabel->setText("Distance from Bottom of Ramp = " + std::to_string(m_deltaXbot) + "(m). Net Force = " + std::to_string(Util::magnitude(m_pLootbox->getNetForce() / m_PPM)) + "(N)");

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
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

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Pixels Per Meter
	m_PPM = PPM;

	m_trianglePos.x = 100;
	m_trianglePos.y = 450;
	m_rise = 3.0f * m_PPM;
	m_run = 4.0f * m_PPM;

	m_pLootbox = new Box();
	m_pLootbox->getTransform()->position = glm::vec2(m_trianglePos.x, m_trianglePos.y - m_rise);
	m_pLootbox->setWidth(50);
	m_pLootbox->setHeight(54);
	m_pLootbox->setPixelsPerMeter(m_PPM);
	m_pLootbox->setGravity(9.8f);
	addChild(m_pLootbox);
	
	m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/restartButton.png", "Reset", RESTART_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
		m_maxVelocity = 0;
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
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pTempLabel = new Label("X from Bot Ramp: ", "Consolas");
	m_pTempLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 20.0f);
	addChild(m_pTempLabel);

	m_maxVelocity = 0;
}

void PlayScene::GUI_Function() const
{
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button((m_pLootbox->IsActive() || m_pLootbox->getTransform()->position.x > m_trianglePos.x ? "Reset Simulation" : "Activate")))
	{
		if (m_pLootbox->IsActive() || m_pLootbox->getTransform()->position.x > m_trianglePos.x)
		{
			m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
		}
		else
		{
			m_pLootbox->toggleActive();
			m_pLootbox->setDiretion(glm::normalize(glm::vec2(m_run, m_rise)));
		}
	}

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

		(float)m_rise = height * m_PPM;
		(float)m_run = length * m_PPM;
		m_pLootbox->setFriction(CoefficientFriction);
		m_pLootbox->getRigidBody()->mass = mass;

		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
		
	}

	ImGui::Separator();

	
	if (ImGui::SliderFloat("Height (m)", &height, 0.01f, 15.0f)) {
		(float)m_rise = height * m_PPM;
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Angle of depression:  %f degrees",  -(glm::degrees(glm::atan(m_rise,m_run))));
	
	if (ImGui::SliderFloat("Length (m)", &length, 0.01f, 20.0f)) {
		(float)m_run = length * m_PPM;
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}

	ImGui::SameLine(350.0F, 1);
	ImGui::Text("Total X displacement:  %fm", (m_pLootbox->getTransform()->position.x - m_trianglePos.x) /m_PPM);
	
	if (ImGui::SliderFloat("Co. of Friction", &CoefficientFriction, 0.0f, 3.0f)) {
		m_pLootbox->setFriction(CoefficientFriction);
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Distance from slope:   %fm", (m_pLootbox->getTransform()->position.x - m_run - m_trianglePos.x)/m_PPM);

	if (ImGui::SliderFloat("Mass (kg)", &mass, 0.1f, 200.0f)) {
		m_pLootbox->getRigidBody()->mass = mass;
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Net Force:              %fN", (Util::magnitude(m_pLootbox->getNetForce() / m_PPM)));
	
	ImGui::Checkbox("Show Net Force", &m_viewForce);

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Acceleration (magnitude):%fm/s^2", (Util::magnitude(m_pLootbox->getRigidBody()->acceleration / m_PPM)));

	ImGui::Checkbox("Show Velocity", &m_viewVelocity);

	ImGui::SameLine(350.0F, -1);
	ImGui::Text("Max Speed:            %fm/s", m_maxVelocity);


	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool PlayScene::StartSim()
{
		if (m_pLootbox->IsActive() || m_pLootbox->getTransform()->position.x > m_trianglePos.x)
		{
			m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
			m_maxVelocity = 0;
			return true;
		}
		else
		{
			m_pLootbox->toggleActive();
			m_pLootbox->setDiretion(glm::normalize(glm::vec2(m_run, m_rise)));
			m_maxVelocity = (Util::magnitude(glm::vec2(m_run, m_rise) / m_PPM) * Util::magnitude(m_pLootbox->getRigidBody()->acceleration / m_PPM));
			return false;
		}
}


void PlayScene::DrawArrow(glm::vec2 Start, glm::vec2 Dir, float Length, glm::vec4 colour)
{
	glm::vec2 EndPos = Start + Dir * Length;

	Util::DrawLine(Start, EndPos, colour);

	// Draw Arrow Head
	glm::vec2 rightArrow = (Util::normalize(glm::vec2(Dir.y, -Dir.x) - Dir)) * 10.0f;
	glm::vec2 leftArrow = (Util::normalize(glm::vec2(-Dir.y, Dir.x) - Dir)) * 10.0f;
	Util::DrawLine(EndPos, EndPos + rightArrow, colour);
	Util::DrawLine(EndPos, EndPos + leftArrow, colour);
}
