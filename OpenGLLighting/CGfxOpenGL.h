#pragma once
#ifndef __GL_COMPONENT
#define __GL_COMPONENT
#define PI 3.14159
#define TWO_PI PI*2.0
#define HALF_PI PI/2.0

class Robot;

class CGfxOpenGL {
private:
	Robot * theRobot;
	int m_windowWidth;
	int m_windowHeight;
	float rotateAngle;
public:
	CGfxOpenGL();
	virtual ~CGfxOpenGL();

	bool Init();
	bool Shutdown();

	void SetupProjection(int width, int height);

	void ResizeScene(int width, int height);//重置视口
	void UpdateProjection(bool toggle = FALSE);//正交投影 或 透视投影

	void DrawCube(float xPos, float yPos, float zPos);

	void Prepare(float dt);
	void Render();
};

#endif // __GL_COMPONENT
