#include "Laborator3.h"
// Lab3.cpp
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;


	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);

	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	Mesh* soare = Object2D::CreateSquare("soare", corner, 100, glm::vec3(1, 0, 0), true);
	AddMeshToList(soare);

	Mesh* pamant = Object2D::CreateSquare("pamant", corner, 50, glm::vec3(0, 1, 0));
	AddMeshToList(pamant);

	Mesh* luna = Object2D::CreateSquare("luna", corner, 20, glm::vec3(0, 0, 1));
	AddMeshToList(luna);

}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


float rotation = 0;
float factorScale = 1;
float translationX = 0;
float scale = 1;
float factor = 1;


float rotationLuna = 0;
float rotationPamant = 0;
float xSoare = 640, ySoare = 360;
float xPamant = 500, yPamant = 350;
float xluna = 550, yluna = 350;

double i = 0;

void Laborator3::Update(float deltaTimeSeconds)
{
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(xSoare, ySoare);
	//modelMatrix *= Transform2D::Rotate(rotation);
	RenderMesh2D(meshes["soare"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(xPamant, yPamant);
	modelMatrix *= Transform2D::Rotate(rotation / 3);
	RenderMesh2D(meshes["pamant"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(xluna, yluna);
	modelMatrix *= Transform2D::Rotate(rotation);
	RenderMesh2D(meshes["luna"], shaders["VertexColor"], modelMatrix);

	if (i >= 2 * AI_MATH_PI_F)
		i = 0;
	else
		i += 1 * deltaTimeSeconds;

	xluna = xPamant + 100 * cos(i * 3);
	yluna = yPamant + 100 * sin(i * 3);


	xPamant = xSoare + 200 * cos(i);
	yPamant = ySoare + 200 * sin(i);

	//cout << xluna << endl << yluna;
	// TODO: update steps for translation, rotation, scale, in order to create animations

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(150, 250);
	modelMatrix *= Transform2D::Translate(translationX, 200);
	// TODO: create animations by multiplying current transform matrix with matrices from Transform 2D

	//RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(400, 250);
	modelMatrix *= Transform2D::Rotate(rotation);
	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D

	//RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(650, 250);
	modelMatrix *= Transform2D::Scale(scale, scale);

	translationX += factor * deltaTimeSeconds * 100;
	if (translationX >= 800 || translationX <= 0)
		factor *= -1;

	rotation += deltaTimeSeconds;

	scale += factorScale * deltaTimeSeconds;
	if (scale > 3 || scale < 0.1)
		factorScale *= -1;

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	//RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}