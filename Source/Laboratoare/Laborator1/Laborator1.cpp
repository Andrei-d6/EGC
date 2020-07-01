#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* mesh2 = new Mesh("sphere");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh2->GetMeshID()] = mesh2;

		Mesh* mesh3 = new Mesh("teapot");
		mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh3->GetMeshID()] = mesh3;
	}
}

void Laborator1::FrameStart()
{

}

int object = 0;
float x = -2, y = 0, z = 0;
float startingy;
int r, g, b;
bool isJumping = false;
bool isFalling = false;

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(r, g, b, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(2, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(x, y, z));

	// render my own sphere
	if (object == 0)
		RenderMesh(meshes["sphere"], glm::vec3(0, 0.5f, 0), glm::vec3(0.5f));
	else if (object == 1)
		RenderMesh(meshes["box"], glm::vec3(0, 0.5f, 0));
	else
		RenderMesh(meshes["teapot"], glm::vec3(0, 0.5f, 0), glm::vec3(0.5f));

	if (isJumping)
	{
		if (y < startingy + 2.3f)
		{
			y += 4 * deltaTimeSeconds;
			z -= 2 * deltaTimeSeconds;
		}
		else
		{
			isJumping = false;
			isFalling = true;
		}
	}
	if (isFalling)
	{
		if (y > startingy)
		{
			y -= 8 * deltaTimeSeconds;
			z -= 2 * deltaTimeSeconds;
		}
		else
		{
			isJumping = false;
			isFalling = false;
		}
	}
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_D)) {
		x += 0.2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		x -= 0.2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		z -= 0.2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		z += 0.2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
		y += 0.2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_E)) {
		y -= 0.2 * deltaTime;
	}
	// treat continuous update based on input
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		if (r == 255)
			r = 0;
		else
			r = 255;
		cout << "Am apasat F\n";

	}
	if (key == GLFW_KEY_C) {
		if (object < 2)
			object++;
		else
			object = 0;
	}


	if (key == GLFW_KEY_SPACE && isJumping == false && isFalling == false) {
		isJumping = true;
		startingy = y;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}