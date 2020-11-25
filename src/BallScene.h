#pragma once
#ifndef __BALL_SCENE__
#define __BALL_SCENE__


#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Polygon.h"

class BallScene : public Scene
{
public:
	BallScene();
	~BallScene();

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

	//slope variables
	glm::vec2 m_trianglePos;
	float m_run, m_rise;

	float m_PPM;

	Polygon* m_ball;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_pTempLabel;
	float m_maxVelocity;	//used for speed label
	static bool m_viewForce;
	static bool m_viewVelocity;

	// Misc Functions
	void DrawArrow(glm::vec2 Start, glm::vec2 Dir, float Length, glm::vec4 colour);
};

#endif /* defined (__BALL_SCENE__) */