#include "stdafx.h"
#include "Robot.h"


void Robot::DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.0f, 0.0f);//top 逆时针画四方形
			glVertex3f(0.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);

			glVertex3f(0.0f, 0.0f, 0.0f);//front
			glVertex3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, -1.0f, 0.0f);

			glVertex3f(0.0f, 0.0f, 0.0f);//right
			glVertex3f(0.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, -1.0f, -1.0f);
			glVertex3f(0.0f, 0.0f, -1.0f);

			glVertex3f(-1.0f, 0.0f, 0.0f);//left
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glVertex3f(0.0f, -1.0f, 0.0f);//bottom
			glVertex3f(0.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glVertex3f(0.0f, 0.0f, -1.0f);//back
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(0.0f, -1.0f, -1.0f);
		glEnd();
	glPopMatrix();
}

void Robot::DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 4.0f, 1.0f);
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::DrawHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(2.0f, 2.0f, 2.0f);
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(3.0f, 5.0f, 2.0f);
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		glPushMatrix();
			glTranslatef(0.0f, -0.5f, .0f);
			DrawFoot(0.0f, -5.0f, 0.0f);
		glPopMatrix();

		glScalef(1.0f, 5.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 0.0f);
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void Robot::DrawFoot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 0.5f, 3.0f);
		DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

Robot::Robot()
{
	armAngles[LEFT] = 0.0;
	armAngles[RIGHT] = 0.0;
	legAngles[LEFT] = 0.0;
	legAngles[RIGHT] = 0.0;

	armStates[LEFT] = FORWARD_STATE;
	armStates[RIGHT] = BACKWARD_STATE;

	legStates[LEFT] = FORWARD_STATE;
	legStates[RIGHT] = BACKWARD_STATE;
}


Robot::~Robot()
{
}

void Robot::Prepare(float dt)
{
	for (char side = 0; side < 2; side++) {
		if (armStates[side] == FORWARD_STATE) {
			armAngles[side] += 20.0f * dt;
		}
		else {
			armAngles[side] -= 20.0f * dt;
		}

		if (armAngles[side] >= 15.0f) {
			armStates[side] = BACKWARD_STATE;
		}
		else if (armAngles[side] <= -15.0f) {
			armStates[side] = FORWARD_STATE;
		}

		if (legStates[side] == FORWARD_STATE) {
			legAngles[side] += 20.0f * dt;
		}
		else {
			legAngles[side] -= 20.0f * dt;
		}

		if (legAngles[side] >= 15.0f) {
			legStates[side] = BACKWARD_STATE;
		}
		else if (legAngles[side] <= -15.0f) {
			legStates[side] = FORWARD_STATE;
		}
	}
}

void Robot::DrawRobot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glTranslatef(xPos, yPos, zPos);
		DrawHead(1.0f, 2.0f, 0.0f);
		DrawTorso(1.5f, 0.0f, 0.0f);

		glPushMatrix();
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(armAngles[LEFT], 1.0f, 0.0f, 0.0f);
			DrawArm(2.5f, 0.0f, -0.5f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
			DrawArm(-1.5f, 0.0f, -0.5f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(legAngles[LEFT], 1.0f, 0.0f, 0.0f);
			DrawLeg(-0.5f, -5.0f, -0.5f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0f, -0.5f, 0.0f);
			glRotatef(legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
			DrawLeg(1.5f, -5.0f, -0.5f);
		glPopMatrix();

	glPopMatrix();
}
