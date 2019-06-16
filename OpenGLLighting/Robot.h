#pragma once

const char BACKWARD_STATE = 0;
const char FORWARD_STATE = 1;

const char LEFT = 0;
const char RIGHT = 1;
class Robot
{
private:

	char legStates[2];
	char armStates[2];

	float legAngles[2];
	float armAngles[2];

	void DrawCube(float xPos, float yPos, float zPos);

	void DrawArm(float xPos, float yPos, float zPos);
	void DrawHead(float xPos, float yPos, float zPos);
	void DrawTorso(float xPos, float yPos, float zPos);
	void DrawLeg(float xPos, float yPos, float zPos);
	void DrawFoot(float xPos, float yPos, float zPos);
public:
	Robot();
	virtual ~Robot();

	void Prepare(float dt);
	void DrawRobot(float xPos, float yPos, float zPos);
};

