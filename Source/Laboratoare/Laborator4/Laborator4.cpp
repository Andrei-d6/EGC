#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = -2.5f;
	translateY = 0.5f;
	translateZ = -1.5f;
	translateX1 = -3.0f;
	translateY1 = 0.5f;
	translateZ1 = -4.5f;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	//modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	if (translateY > 2)
		sign1 = -1;
	if (translateY < 0.5f)
		sign1 = 1;
	translateY += sign1 * deltaTimeSeconds;
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	if (scaleX > 2)
		sign2 = -1;
	if (scaleX < 1)
		sign2 = 1;
	scaleX += sign2 * deltaTimeSeconds * 0.5f;
	scaleY += sign2 * deltaTimeSeconds * 0.5f;
	scaleZ += sign2 * deltaTimeSeconds * 0.5f;
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	//modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	//modelMatrix *= Transform3D::Translate(-2.5f, -0.5f, +1.5f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(translateX1, translateY1, translateZ1);
	modelMatrix *= Transform3D::Translate(1.5f, 0.0f, 0.0f);
	angularStepOZ1 -= deltaTimeSeconds * 0.5f;
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ1);
	if (angularStepOZ1 < -3.1415)
	{
		translateX1 += 3.0f;
		angularStepOZ1 = 0;
	}
	modelMatrix *= Transform3D::Translate(-1.5f, 0.0f, 0.0f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// TODO
	if (window->KeyHold(GLFW_KEY_3))
		angularStepOX += deltaTime;
	if (window->KeyHold(GLFW_KEY_4))
		angularStepOX -= deltaTime;
	if (window->KeyHold(GLFW_KEY_5))
		angularStepOY += deltaTime;
	if (window->KeyHold(GLFW_KEY_6))
		angularStepOY -= deltaTime;
	if (window->KeyHold(GLFW_KEY_7))
		angularStepOZ += deltaTime;
	if (window->KeyHold(GLFW_KEY_8))
		angularStepOZ -= deltaTime;
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
