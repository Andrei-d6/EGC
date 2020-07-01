#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <stb/stb_image.h>
#include <Core/GPU/Texture2D.h>
#include <stdlib.h>

#include "Camera.h"

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	//Render with Phong Illumination
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
	//Render with Phong Illumination and Texture
	void RenderSimpleMeshWithTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
	//Render With Texture but no color
	void RenderSky(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

	int  GetIndex(float x, float z);
	void SetThirdPersonCamera();

	void LoadHeightMap();
	void ReloadHeightMap();


	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	//Pozitia sursei principale de lumina
	glm::vec3 lightPosition;
	//Directia luminii pentru sursa principala
	glm::vec3 lightDirection;

	//Pozitia luminii de tip spotlight 1
	glm::vec3 spotlight1Position;
	//Directia luminii de tip spotlight 1
	glm::vec3 spotlight1Direction;

	//Pozitia luminii de tip spotlight 2
	glm::vec3 spotlight2Position;
	//Directia luminii de tip spotlight 1
	glm::vec3 spotlight2Direction;


	//Pozitia proiectilului
	glm::vec3 projectilePosition;
	//Directia proiectilului
	glm::vec3 projectileDirection;

	//Pozitia caracterului 1
	glm::vec3 player1Position;
	//Gradul de rotatia al caracterului 1
	glm::vec2 player1RotationFactor;

	//Pozitia caracterului 1
	glm::vec3 player2Position;
	//Gradul de rotatia al caracterului 1
	glm::vec2 player2RotationFactor;


	//Light & material properties
	unsigned int materialShininess;
	float materialKd;
	float materialKs;
	int lightingType = 0;
	float cutoff = 0.4552;

	GLenum polygonMode;

	//HeightMap properties and texture list
	std::unordered_map<std::string, Texture2D*> mapTextures;
	int width = 256;
	int height = 256;
	int channels = 1;
	unsigned char *heightPixels = NULL;

	//Randul curent: 1 - player 1; 2 - player 2; 3 - debug
	int	turn;
	
	//Camera pentru player 1
	Laborator::MyCameraTema3* camera1;
	//Camera pentru player 2
	Laborator::MyCameraTema3* camera2;

	//Extremitatile planului incarcat
	float maxPlaneX, maxPlaneZ;
	float minPlaneX, minPlaneZ;

	//Semafor pentru verificarea lansarii proiectilului
	bool projectileAnimation = false;
	
	//Gradul de rotatie al cerului
	float t;
	//Semafor pentru rotatia cerului: 1 - rotire; 2 - stationar
	int spin = 1;

	//Gradul de cadere al proiectilului
	float fallOff;
	//Delay pentru vizualizarea deformarii terenului
	float timer = 0;
	//Semafor pentru initializarea delay-ului pentru deformarea terenului
	bool startTimer;

	//Only for debugging
	float rotationX = 0;
	float rotationY = 0;


};
