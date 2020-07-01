#include "Tema1.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "../Laborator3/Object2D.h"
#include "../Laborator3/Transform2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

vector <glm::vec3> pipes; //Lista numerelor care descriu perechile de pipe-uri
vector <glm::vec3> pipeSlidingFactor; //Lista numerelor care descriu miscarea verticala a pipe-urilor

vector <glm::vec3> clouds; //Lista de parametrii care descriu coordonatele norilor
vector <glm::vec2> cloudsOffsets; //Lista de offset-uri ale formelor din cadrul norilor

vector <glm::vec3> stars; //Lista de parametrii pentru descrierea stelelor

vector <glm::vec3> hills; //Lista de parametrii pentru construirea dealurilor din fata
vector <glm::vec3> hillsBack; //Lista de parametrii pentru construirea delurilor din spate
vector <glm::vec2> bgPipes; //Lista de parametrii pentru construirea pipe-urilor din fundal

void Tema1::InitBackgroundPipes()
{	
	{	
		//RGB for the day background Pipes
		bgPipeDayRGB.x = 69.0f / 255.0f;
		bgPipeDayRGB.y = 196.0f / 255.0f;
		bgPipeDayRGB.z = 160.0f / 255.0f;
	}

	{
		//RGB for the noon background Pipes
		bgPipeNoonRGB.x = 173.0f / 255.0f;
		bgPipeNoonRGB.y = 155.0f / 255.0f;
		bgPipeNoonRGB.z = 116.0f / 255.0f;
	}

	{
		//RGB for the night background Pipes
		bgPipeNightRGB.x = 94.0f / 255.0f;
		bgPipeNightRGB.y = 94.0f / 255.0f;
		bgPipeNightRGB.z = 150.0f / 255.0f;
	}

	//Date generale care descriu pipe-urile din background
	bgPipeDistance = 350.0f;
	nrOfBgPipes = resolution.x / bgPipeDistance + 2.0f;
	bgPipesSpeed = 130.0f;
	bgPipes.resize(nrOfBgPipes);
	bgPipeWidth = 50;

	int sign;
	sign = rand() % 2;
	if (sign == 0)
		sign = -1;

	//Offset fata de marginea din stanga a ecranului
	float initialOffset = (resolution.x / 6.0f) + sign * (rand() % 10);

	/*
		Initializarea coordonatelor pentru pipe-urile din background
		bgPipes[i].x - coordonata X pentru pipe-ul i din background
		bgPipes[i].y - inaltimea la care va ajunge pipe-ul i din background
	*/
	for (int i = 0; i < nrOfBgPipes; i++)
	{
		bgPipes[i].x = bgPipeDistance * i + initialOffset;
		do
		{
			bgPipes[i].y = rand() % 10 + rand() %100 / 100.0f;
		} while (bgPipes[i].y < 4.5f || bgPipes[i].y >= 8.0f);
	}
}

void Tema1::InitStars()
{	
	//Date generale ale stelelor
	starsSpeed = 30;
	nrOfStars = (int) ((40.0f * resolution.x) / (resolution.y));
	nrOfStars = (int) (nrOfStars / 2.0f);
	stars.resize(nrOfStars);

	/*
		Initializarea perechilor de numere care descriu stelele
		stars[i].x - coordonata X a centrului stelei i 
		stars[i].y - coordonata Y a centrului stelei i
		stars[i].z - factorul de scalare al stelei i
	*/
	for (int i = 0; i < nrOfStars; i++)
	{	
		do
		{
			stars[i].x = rand() % resolution.x + rand() % 10 / 10;
		} while (stars[i].x < 5);

		do
		{
			stars[i].y = resolution.y / 3.0f + rand() % (int)(resolution.y / 1.5f);
		} while (stars[i].y > resolution.y - 10 || stars[i].y < resolution.y / 3.0f);
			
		stars[i].z = 0.08f + (rand() % 10 / 100.0f);
	}
}

void Tema1::InitBackground()
{	
	{	
		//Day RGB for the front hills
		frontHillsRGBDay.x = 25.0f / 255.0f;
		frontHillsRGBDay.y = 178.0f / 255.0f;
		frontHillsRGBDay.z = 142.0f / 255.0f;
	}
	
	{	
		//Noon RGB for the fornt hills
		frontHillsRGBNoon.x = 150.0f / 255.0f;
		frontHillsRGBNoon.y = 160.0f / 255.0f;
		frontHillsRGBNoon.z = 100.0f / 255.0f;
	}

	{	
		//Night rgb for the front hills
		frontHillsRGBNight.x = 72.0f /255.0f;
		frontHillsRGBNight.y = 72.0f / 255.0f;
		frontHillsRGBNight.z = 117.0f / 255.0f;
	}

	{	
		//Day rgb for the back hills
		backHillsRGBDay.x = 113.0f / 255.0f;
		backHillsRGBDay.y = 204.0f / 255.0f;
		backHillsRGBDay.z = 202.0f / 255.0f;
	}

	{
		//Noon RGB for the back hills
		backHillsRGBNoon.x = 200.0f / 255.0f;
		backHillsRGBNoon.y = 160.0f / 255.0f;
		backHillsRGBNoon.z = 120.0f / 255.0f;
	}

	{	
		//Night rgb for the back hills
		backHillsRGBNight.x = 111.0f / 255.0f;
		backHillsRGBNight.y = 111.0f / 255.0f;
		backHillsRGBNight.z = 178.0f / 255.0f;
	}

	//Offset-ul fata de marginea inferioara a ecranului (de inmultit cu latimea backgroundului - bacgroundWidth)
	frontHills_YOffset = 0.55f;
	backHills_YOffset = 1.5f;

	//Date generale ale background-ului
	hillSpeed = 150;
	hillBackSpeed = 100;
	backgroundLength = resolution.x;
	backgroundWidth = resolution.y / 7.0f;
	hillRadius = 25.0f;
	nrOfHills = backgroundLength / hillRadius;
	nrOfHills *= 1.5f;
	hills.resize(nrOfHills);
	hillsBack.resize(nrOfHills);

	float hillX = 0.0f; //variabila auxiliara pentru a mentine initial echidistante centrele cercurilor care descriu backgroundul
	
	/*
		Initializarea perechilor de numere care descriu cercurile din background
		hills[i].x = coordonata X a centrului cercului i - front
		hills[i].y = coordonata Y a centrului cercului i - front
		hills[i].z = coeficientul de scalare al cercului i - front


		hillsBack[i].x = coordonata X a centrului cercului i - back
		hillsBack[i].y = coordonata Y a centrului cercului i - back
		hillsBack[i].z = coeficientul de scalare al cercului i - back
	*/
	for (int i = 0; i < nrOfHills; i++)
	{
		hills[i].x = hillX; //x hill coordinate
		hills[i].y = backgroundWidth + rand() % ((int)(hillRadius / 1.2f)); // y hill coordinate
		hills[i].z = rand() % 4 + ((float)(rand() % 100)) / 100.0f; // hill scaling factor

		if (hills[i].z <= 3)
			hills[i].z += 3.2f;

		hillsBack[i].x = hillX; //x hill coordinate
		hillsBack[i].y = backgroundWidth + rand() % ((int)(hillRadius / 2.0f)); // y hill coordinate
		hillsBack[i].z = rand() % 4 + ((float)(rand() % 100)) / 100.0f; // hill scaling factor

		if (hillsBack[i].z < 1)
			hillsBack[i].z += 1.2f;

		hillX += 2.0f * hillRadius;
	}
}

void Tema1::InitClouds()
{	
	//Date generale ale norilor
	cloudSpeed = 180.0f;
	cloudRadius = 25.0f;
	nrOfClouds = 5;
	clouds.resize(nrOfClouds);
	cloudsOffsets.resize(2 * nrOfClouds);

	/*
		Initializarea perechilor de numere ce descriu norii
		clouds[i].x - coordonata X a norului i (a centrului cercului de referinta din norul i)
		clouds[i].y - coordonata Y a norului i (a centrului cercului de referinta din norul i)
		clouds[i].z - un numar intre 0 si nrOfClouds unde 0 presupune inexistenta unui nor iar celelalte numere determina forma norului
	*/
	for (int i = 0; i < nrOfClouds; i++)
	{
		clouds[i].z = rand() % (int)nrOfClouds;

		if (clouds[i].z != 0)
		{	
			clouds[i].x = resolution.x + rand() % (int)(resolution.x);
			int sign = rand() % 2;
			if (sign == 0)
				sign = 1;
			else
				sign = -1;
			clouds[i].y = resolution.y / 2.0f + sign * (rand() % (int)(resolution.y / 2.0f) +  (float)(rand() % 10 / 10.0f));
			
			if (clouds[i].y < resolution.y / 4.0f)
				clouds[i].y = resolution.y + rand() % (int)(resolution.y / 2.0f);
		}
	}

	//Date ce descriu pozitiile relative ale cercurilor din cadrul formelor de nor 
	//(fata de centrul cercului de referinta al norului)
	{
		cloudsOffsets[0].x = 3.0f * cloudRadius;    cloudsOffsets[0].y = 10.0f;
		cloudsOffsets[1].x = 5.0f * cloudRadius;    cloudsOffsets[1].y = -10.0f;
		cloudsOffsets[2].x = cloudRadius;           cloudsOffsets[2].y = -10.0f;
		cloudsOffsets[3].x = cloudRadius;           cloudsOffsets[3].y = 0.5f * cloudRadius;

		cloudsOffsets[4].x = 1.3f * cloudRadius;	cloudsOffsets[4].y = 4.0f;
		cloudsOffsets[5].x = 2.8f * cloudRadius;	cloudsOffsets[5].y = -1.0f;
	}
	
}

void Tema1::InitPipes()
{	
	//Date generale ale stelelor
	pipeDistance = 260.0f;
	nrOfPipes = resolution.x / pipeDistance + 2.0f;

	pipes.resize(nrOfPipes);
	pipeSlidingFactor.resize(nrOfPipes);
	maxSlidingFactor = 1.5f;

	pipeWidth = 80.0f;
	pipeRadius = pipeWidth / 2.0f;
	minGapHeight = 180.0f;
	maxGapHeight = 400.0f;
	pipeSpeed = 200.0f;
	pipeEnd_scalingFactorX = 1.5f;
	pipeEnd_scalingFactorY = 0.8;
	firstPipe = true;

	//Offset-ul initial fata de marginea stanga a ecranului
	float initialOffset = resolution.x / (2 * (pipeDistance + pipeWidth));
	
	/*	
		Initializarea datelor ce descriu glisarea perechilor de stalagmite si stalactite
		pipeSlidingFactor[i].x - viteza de glisare a perechilor de stalagmite si stalactite
		pipeSlidingFactor[i].y - distanta de glisare a perechilor de stalagmite si stalactite
		pipeSlidingFactor[i].z - 0 sau 1; numar ce descrie daca acestea gliseaza ascendent sau descendent

		Initializarea datelor ce descriu coordonatele si marimea pipe-urilor
		pipes[i].x - coordonata X a pipe-ului i
		pipes[i].y - factorul de scalare relativ la marignea inferioara a ecranului a stalagmitei i
		pipes[i].z - distanta dintre stalagmita si stalactita i
	*/
	for (int i = 0; i < nrOfPipes; i++)
	{	
		do
		{
			pipeSlidingFactor[i].x = 1 + rand() % 100 / 100.0f;
		} while (pipeSlidingFactor[i].x < 1 || pipeSlidingFactor[i].x > 1.5f);

		do
		{
			pipeSlidingFactor[i].y = rand() % 10 + 1;
		} while (pipeSlidingFactor[i].y <= 1 || pipeSlidingFactor[i].y > 7);
		
		pipeSlidingFactor[i].z = rand() % 2;

		pipes[i].x = pipeDistance * (i + initialOffset);
		do
		{
			pipes[i].y = rand() % (resolution.y / 110) + (float)(rand() % 100) / 100.0f;
		} while (pipes[i].y < 0.5f);

		float gap;
		do
		{
			gap = rand() % (int)maxGapHeight + (float )(rand() % 100) / 100.0f;
		} while (gap < minGapHeight);

		if (pipeWidth * pipes[i].y + gap >= resolution.y * 3.0f / 4.0f)
		{
			pipes[i].y /= 2.0f;
		}

		pipes[i].z = gap;
	}
}

void Tema1::InitConstants()
{	
	{	
		//Culoarea cerului pe timpul zilei
		skyDay.x = 169.0f / 255.0f;
		skyDay.y = 209.0f / 255.0f;
		skyDay.z = 252.0f / 255.0f;
	}

	{	
		//Culoarea cerului pe timp de seara
		skyNoon.x = 1.0f;
		skyNoon.y = 180.0f / 255.0f;
		skyNoon.z = 120.0f / 255.0f;
	}

	{
		//Culoarea cerului pe timp de noapte
		skyNight.x = 39.0f / 255.0f;
		skyNight.y = 38.0f / 255.0f;
		skyNight.z = 73.0f / 255.0f;
	}

	skyRGB = skyDay;
	screenOffset = resolution.y / 2.0f - 10.0f;
	gravity = 2000.0f;
	score = 0;

	timeCounter = 0;
	timeCap = 5;
	day = true;
	noon = false;
	night = false;
}

void Tema1::InitBird()
{	
	{	
		//Culoarea corpului pasarii
		birdBodyRGB.x = 255.0f / 255.0f;
		birdBodyRGB.y = 220.0f / 255.0f;
		birdBodyRGB.z = 0.0f / 255.0f;
	}

	{	
		//Culoarea capului pasarii
		birdHeadRGB.x = 1.0f;
		birdHeadRGB.y = 230.0f / 255.0f;
		birdHeadRGB.z = 10.0f / 255.0f;
	}

	{	
		//Culoarea ciocului (si a aripii) pasarii
		birdBeakRGB.x = 0.87f;
		birdBeakRGB.y = 70.0f / 255.0f;
		birdBeakRGB.z = 10.0f / 255.0f;
	}

	birdX = (float)resolution.x / 6.0f;
	birdY = (float)resolution.y / 2.0f;
	birdPosition = 0.0f;
	birdVelocity = 0.0f;
	birdAcceleration = 0.0f;
	birdPosition = 0;
	birdRadius = 28.0f;
	beak_XOffset = 24.0f;
	beak_YOffset = -22.0f;
	birdHead_XOffset = 0.0f;
	birdHead_YOffset = -22.0f;
	
	birdBodyRadius = 23.0f;
	birdHeadRadius = 17.0f;
	birdEyeRadius = 3.25f; // old is 2.5f

	beakLength = 12.0f;

	wing_transaltionOffset = 5.0f;
	wing_XOffset = -20.0f;
	wing_YOffset = 0.0f;
	wing_XScalingFactor = 1.5f;
	wing_YScalingFactor = 1.2f;

	eye_XOffset = 29;
	eye_YOffset = 9;

	birdCenterX = birdX;
	birdCenterY = resolution.y / 2.0f + birdPosition;
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	InitConstants();
	InitPipes();
	InitBird();
	InitClouds();
	InitBackground();
	InitBackgroundPipes();

	glm::vec3 center = glm::vec3((float)resolution.x / 6.0f, (float)resolution.y / 2.0f, 0);
	Mesh* birdBody = Object2D::CreateDisk("birdBody", center, birdBodyRadius, birdBodyRGB, birdHeadRGB, true);
	AddMeshToList(birdBody);

	center = glm::vec3(25.0f, 25.0f, 0);
	Mesh* birdHead = Object2D::CreateDisk("birdHead", center, birdHeadRadius, birdHeadRGB, birdHeadRGB, true);
	AddMeshToList(birdHead);

	Mesh* beak = Object2D::CreateTriangle("beak", center, beakLength, birdBeakRGB, true);
	AddMeshToList(beak);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	Mesh* pipe = Object2D::CreateRectangle("pipe", corner, pipeWidth, pipeWidth, glm::vec3(0.01f, 0.12f, 0.04f), glm::vec3(0.85f, 1, 0), true);
	AddMeshToList(pipe); 


	Mesh* bgPipeDay = Object2D::CreateRectangle("bgPipeDay", corner, bgPipeWidth, bgPipeWidth, bgPipeDayRGB, bgPipeDayRGB, true);
	AddMeshToList(bgPipeDay);

	Mesh* bgPipeNoon = Object2D::CreateRectangle("bgPipeNoon", corner, bgPipeWidth, bgPipeWidth, bgPipeNoonRGB, bgPipeNoonRGB, true);
	AddMeshToList(bgPipeNoon);

	Mesh* bgPipeNight = Object2D::CreateRectangle("bgPipeNight", corner, bgPipeWidth, bgPipeWidth, bgPipeNightRGB, bgPipeNightRGB, true);
	AddMeshToList(bgPipeNight);
	/*
	Mesh* debugDisk = Object2D::CreateDisk("debugDisk", corner, pipeWidth / 2.0f, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), true);
	AddMeshToList(debugDisk);

	Mesh* debugDisk2 = Object2D::CreateDisk("debugDisk2", corner, birdRadius, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), true);
	AddMeshToList(debugDisk2);
	Mesh* square = Object2D::CreateSquare("square", corner, 10, glm::vec3(1, 0, 0), true);
	AddMeshToList(square);
	*/

	Mesh* cloud = Object2D::CreateDisk("cloud", corner, cloudRadius, glm::vec3(0.95f, 0.95f, 0.95f), glm::vec3(0.8f, 0.9f, 0.9f), true);
	AddMeshToList(cloud);
	Mesh* eye = Object2D::CreateDisk("eye", corner, birdEyeRadius, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), true);
	AddMeshToList(eye);

	//Background Front
	Mesh* bgFrontDay = Object2D::CreateRectangle("bgFrontDay", corner, backgroundLength, backgroundWidth, frontHillsRGBDay, frontHillsRGBDay, true);
	AddMeshToList(bgFrontDay);
	Mesh* bgFrontNoon = Object2D::CreateRectangle("bgFrontNoon", corner, backgroundLength, backgroundWidth, frontHillsRGBNoon, frontHillsRGBNoon, true);
	AddMeshToList(bgFrontNoon);
	Mesh* bgFrontNight = Object2D::CreateRectangle("bgFrontNight", corner, backgroundLength, backgroundWidth, frontHillsRGBNight, frontHillsRGBNight, true);
	AddMeshToList(bgFrontNight);

	//Background Back
	Mesh* bgroundBackDay = Object2D::CreateRectangle("bgroundBackDay", corner, backgroundLength, 1.5f * backgroundWidth, backHillsRGBDay, backHillsRGBDay, true);
	AddMeshToList(bgroundBackDay);
	Mesh* bgBackNoon = Object2D::CreateRectangle("bgBackNoon", corner, backgroundLength, 1.5f * backgroundWidth, backHillsRGBNoon, backHillsRGBNoon, true);
	AddMeshToList(bgBackNoon);
	Mesh* bgBackNight = Object2D::CreateRectangle("bgBackNight", corner, backgroundLength, 1.5f * backgroundWidth, backHillsRGBNight, backHillsRGBNight, true);
	AddMeshToList(bgBackNight);

	//Front Hills
	Mesh* frontHillsDay= Object2D::CreateDisk("frontHillsDay", corner, cloudRadius, frontHillsRGBDay, frontHillsRGBDay, true);
	AddMeshToList(frontHillsDay);
	Mesh* frontHillsNoon = Object2D::CreateDisk("frontHillsNoon", corner, cloudRadius, frontHillsRGBNoon, frontHillsRGBNoon, true);
	AddMeshToList(frontHillsNoon);
	Mesh* frontHillsNight = Object2D::CreateDisk("frontHillsNight", corner, cloudRadius, frontHillsRGBNight, frontHillsRGBNight, true);
	AddMeshToList(frontHillsNight);

	//Back Hills
	Mesh* backHillsDay = Object2D::CreateDisk("backHillsDay", corner, cloudRadius, backHillsRGBDay, backHillsRGBDay, true);
	AddMeshToList(backHillsDay);
	Mesh* backHillsNoon = Object2D::CreateDisk("backHillsNoon", corner, cloudRadius, backHillsRGBNoon, backHillsRGBNoon, true);
	AddMeshToList(backHillsNoon);
	Mesh* backHillsNight = Object2D::CreateDisk("backHillsNight", corner, cloudRadius, backHillsRGBNight, backHillsRGBNight, true);
	AddMeshToList(backHillsNight);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(skyRGB.x, skyRGB.y, skyRGB.z, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

bool Tema1::Collision()
{	
	float distance, dx, dy;
	birdCenterX = birdX;
	birdCenterY = resolution.y/2.0f + birdY;
	float errorOffset = 10.0f;

	for (int index = 0; index < nrOfPipes; index++)
	{	
		//if (pipes[index].x <= birdX + birdRadius + errorOffset || pipes[index].x >= birdX - errorOffset)
		
		//Verificarea coliziunii pentru pipe-ul din vecinatatea pasarii
		if (pipes[index].x <= birdX + birdRadius + 10 * errorOffset && pipes[index].x >= birdX - 10 * errorOffset)
		{	
			//Check for bottom collision
			for (float bottomCenterY = 0; bottomCenterY < (float)pipeWidth * pipes[index].y - pipeRadius; bottomCenterY += pipeRadius / 2.0f)
			{	
				dx = (pipes[index].x + pipeRadius) - birdCenterX;
				dy = bottomCenterY - birdCenterY;

				distance = sqrt(dx * dx + dy * dy);

				if (distance + 0.2f < birdRadius + pipeRadius)
				{
					//std::cout << "Bottom PIPE   " <<  index  <<endl;
					return true;
				}
			}
			//Check the bottom pipe end collision
			dx = (pipes[index].x - 0.25f * pipeWidth + pipeRadius) - birdCenterX;
			dy = pipeWidth * pipes[index].y - pipeRadius - birdCenterY;

			distance = sqrt(dx * dx + dy * dy);

			if (distance + 0.2f < birdRadius + pipeRadius)
			{
				//std::cout << "Bottom PIPE END Left   " << index << endl;
				return true;
			}

			dx = (pipes[index].x - 0.25f * pipeWidth + pipeWidth) - birdCenterX;
			dy = (pipeWidth * pipes[index].y - pipeRadius) - birdCenterY;

			distance = sqrt(dx * dx + dy * dy);

			if (distance + 0.2f < birdRadius + pipeRadius)
			{
				//std::cout << "Bottom PIPE END Right   " << index << endl;
				return true;
			}
			//End of bottom pipe Collision

			//Check for top collision
			for (float topCenterY = pipeWidth * pipes[index].y + pipes[index].z + pipeRadius + 5.0f; topCenterY <= resolution.y + pipeWidth; topCenterY += pipeRadius / 2.0f)
			{	
				dx = (pipes[index].x + pipeRadius) - birdCenterX;
				dy = topCenterY - birdCenterY;

				distance = sqrt(dx * dx + dy * dy);

				if (distance + 0.2f < birdRadius + pipeRadius)
				{
					//std::cout << "Top PIPE   " << index << endl;
					return true;
				}
			}

			//Check the top pipe end collision
			dx = (pipes[index].x - 0.25f * pipeWidth + pipeRadius) - birdCenterX;
			dy = (pipeWidth * pipes[index].y + pipes[index].z + pipeRadius) - birdCenterY;

			distance = sqrt(dx * dx + dy * dy);

			if (distance + 0.2f < birdRadius + pipeRadius)
			{
				//std::cout << "Top PIPE END Left  " << index << endl;
				return true;
			}

			dx = (pipes[index].x - 0.25f * pipeWidth + pipeWidth) - birdCenterX;
			dy = (pipeWidth * pipes[index].y + pipes[index].z + pipeRadius) - birdCenterY;

			distance = sqrt(dx * dx + dy * dy);

			if (distance + 0.2f < birdRadius + pipeRadius)
			{
				//std::cout << "Top PIPE END Right   " << index << endl;
				return true;
			}
			//End of top pipe Collision

			break;
		}
		
	}
	return false;
}

void Tema1::Update(float deltaTimeSeconds)
{	
	if (startGame)
	{
		//Pipes
		for (int i = 0; i < nrOfPipes; i++)
		{
			if (pipes[i].x > -pipeWidth)
			{	
				//Actualizarea potiei pipe-ului
				pipes[i].x -= pipeSpeed * deltaTimeSeconds;

				//Pipe-urile incep sa se miste in sus si jos de la un anumit prag
				if (score >= 15)
				{	
					//pipeSlidingFactor[i].z == 0 => up
					//pipeSlidingFactor[i].z == 1 => down
					if (pipeSlidingFactor[i].y > 0 && pipes[i].y * pipeWidth >= pipeWidth / 2.0f && pipeSlidingFactor[i].z == 1)
					{
						pipes[i].y -= pipeSlidingFactor[i].x * deltaTimeSeconds;
						pipeSlidingFactor[i].y -= 1.1f * deltaTimeSeconds;
					}
					else if (pipeSlidingFactor[i].y > 0 && pipes[i].y * pipeWidth + pipes[i].z < resolution.y - (1 + pipeEnd_scalingFactorY) * pipeWidth && pipeSlidingFactor[i].z == 0)
					{
						pipes[i].y += pipeSlidingFactor[i].x * deltaTimeSeconds;
						pipeSlidingFactor[i].y -= 1.1f * deltaTimeSeconds;
					}
					else
					{	
						//Reinitializarea miscarii de translatie pe verticala dupa terminarea unei curse
						// (de jos in sus sau de sus in jos)
						do
						{
							pipeSlidingFactor[i].x = 1 + rand() % 100 / 100.0f;
						} while (pipeSlidingFactor[i].x < 1 || pipeSlidingFactor[i].x > maxSlidingFactor);

						do
						{
							pipeSlidingFactor[i].y = rand() % 10 + 1;
						} while (pipeSlidingFactor[i].y <= 1 || pipeSlidingFactor[i].y > 7);
						pipeSlidingFactor[i].z = 1 - pipeSlidingFactor[i].z;
					}
				}

				//Ajustarea scorului pentru prima trecere printr-o pereche de pipe-uri
				if (firstPipe && pipes[i].x < resolution.x / 6.0f - 1.25f * pipeWidth)
				{
					firstPipe = false;
					score++;
					std::cout << "SCORE: " << score << endl;
					timeCounter++;
				}
			}
			else
			{	
				//Pipe-ul cel mai din stanga aproape a parasit fereastra si trebuie reinitializat
				pipes[i].x = nrOfPipes * pipeDistance - pipeWidth;
				do
				{
					pipes[i].y = rand() % (resolution.y / 110) + (float)(rand() % 100) / 100.0f;
				} while (pipes[i].y < 0.5f);

				float gap;
				do
				{
					gap = rand() % (int)maxGapHeight + (float)(rand() % 100) / 100.0f;
				} while (gap < minGapHeight);

				pipes[i].z = gap;

				if (pipeWidth * pipes[i].y + gap >= resolution.y * 3.0f / 4.0f)
				{
					pipes[i].y /= 2.0f;
				}

				//Pasarea a mai trecut printr-o pereche de stalagmite si stalactite => scorul trebuie actualizat
				score++;
				timeCounter++;
				std::cout << "SCORE: " << score << endl;

				//Odata la trecerea printr-un numar dat de perechi de stalacmite si stalactite
				//se actualizeaza momentul curent al zilei
				if (timeCounter % timeCap == 0)
				{
					timeCounter = 0;
					if (day) //a fost zi => seara
					{	
						skyRGB = skyNoon;
						noon = true;
						day = night = false;
					}
					else if (noon) //a fost seara => noapte
					{
						night = true;
						skyRGB = skyNight;
						InitStars();
						day = noon = false;
					}
					else if (night) // a fost noapte => zi
					{	
						maxSlidingFactor += 0.1f;
						skyRGB = skyDay;
						day = true;
						noon = night = false;
					}
				}

				//Pe masura ce avanseaza jocul viteza de deplasare creste
				if (score % 2 == 0 && score != 0)
				{
					pipeSpeed += 5.0f;
					cloudSpeed += 3.0f;

					if (maxGapHeight > 300.0f)
						maxGapHeight -= 0.5f;
				}
			}
		}

		//Actualizarea pozitiei relative a pasarii
		birdAcceleration += 1.8f * gravity * deltaTimeSeconds;

		if (birdAcceleration >= gravity)
			birdAcceleration = gravity;

		birdVelocity += birdAcceleration * deltaTimeSeconds;
		birdPosition -= birdVelocity * deltaTimeSeconds;
		
		//Clouds
		if (!night)
		{
			for (int i = 0; i < nrOfClouds; i++)
			{	
				if (clouds[i].x > -6 * cloudRadius && clouds[i].z != 0)
				{	
					//Actualizarea pozitiei norilor
					clouds[i].x -= cloudSpeed * deltaTimeSeconds;
				}
				else
				{	
					//Un nor a parasit fereastra => acesta trebuie reinitializat
					clouds[i].z = rand() % (int)nrOfClouds;

					if (clouds[i].z != 0)
					{
						clouds[i].x = resolution.x + rand() % (int)(resolution.x);
						int sign = rand() % 2;
						if (sign == 0)
							sign = 1;
						else
							sign = -1;
						clouds[i].y = resolution.y / 2.0f + sign * (rand() % (int)(resolution.y / 2.0f) + (float)(rand() % 10 / 10.0f));

						if (clouds[i].y < resolution.y / 4.0f)
							clouds[i].y = resolution.y + rand() % (int)(resolution.y / 2.0f);
					}
				}
			}
		}

		//Background
		for (int i = 0; i < nrOfHills; i++)
		{
			if (hills[i].x > -2.0f * hills[i].z * hills[i].y)
			{	
				//Actualizarea potiei dealurilor (front)
				hills[i].x -= hillSpeed * deltaTimeSeconds;
			}
			else
			{	
				//Un deal a parasit fereastra => acesta trebuie reinitializat
				hills[i].y = backgroundWidth + rand() % ((int)(hillRadius / 1.2f)); // y hill coordinate
				hills[i].z = rand() % 4 + ((float)(rand() % 100)) / 100.0f; // hill scaling factor

				if (hills[i].z <= 3)
					hills[i].z += 3.2f;

				if (i != 0)
					hills[i].x = hills[i - 1].x + 2.0f * hillRadius;
				else
					hills[i].x = hills[nrOfHills - 1].x + 2.0f * hillRadius;
			}

			if (hillsBack[i].x > -2.0f * hillsBack[i].z * hillsBack[i].y)
			{	
				//Actualizarea potiei dealurilor (back)
				hillsBack[i].x -= hillBackSpeed * deltaTimeSeconds;
			}
			else
			{	
				//Un deal a parasit fereastra => acesta trebuie reinitializat
				hillsBack[i].y = backgroundWidth + rand() % ((int)(hillRadius / 2.0f)); // y hill coordinate
				hillsBack[i].z = rand() % 4 + ((float)(rand() % 100)) / 100.0f; // hill scaling factor

				if (hillsBack[i].z < 1)
					hillsBack[i].z += 1.2f;

				if (i != 0)
					hillsBack[i].x = hillsBack[i - 1].x + 2.0f * hillRadius;
				else
					hillsBack[i].x = hillsBack[nrOfHills - 1].x + 2.0f * hillRadius;
			}
		}

		//Background Pipes
		for (int i = 0; i < nrOfBgPipes; i++)
		{
			if (bgPipes[i].x > -bgPipeWidth)
			{	
				//Actualizarea pozitiei pipe-urilor (stalagmitelor) din fundal
				bgPipes[i].x -= bgPipesSpeed * deltaTimeSeconds; 
			}
			else
			{	
				//Reinitializarea pipe-urilor (stalagmitelor) din fundal
				bgPipes[i].x = nrOfBgPipes * bgPipeDistance - bgPipeWidth;
				do
				{
					bgPipes[i].y = rand() % 10 + rand() % 100 / 100.0f;
				} while (bgPipes[i].y < 4.5f || bgPipes[i].y >= 8.0f);
			}
		}

		//Stars
		if (night)
		{
			for (int i = 0; i < nrOfStars; i++)
			{
				if (stars[i].x > 0)
				{	
					//Deplasarea usoara a stelelor pe parcursul noptii
					stars[i].x -= starsSpeed * deltaTimeSeconds;
				}
				else
				{	
					//Reinitializarea stelelor pe masura ce acestea parasesc fereastra
					stars[i].x = resolution.x / 2.0f + rand() % resolution.x + rand() % 10 / 10;
					stars[i].y = resolution.y / 3.0f + rand() % (int)(resolution.y / 1.5f);
					stars[i].z = 0.08f + (rand() % 10 / 100.0f);
				}
			}
		}
		
	}
	
	//Verificarea coliziunii dupa actualizarea pozitiilor elementelor din ecran
	if (Collision())
	{	
		startGame = false;
		gameOver = true;
		if (displayedGameOver == false)
		{
			displayedGameOver = true;
			std::cout << "		Game Over" << endl;
			std::cout << "Attempts: " << nrOfAttempts << endl << endl;
			std::cout << "You final SCORE is: " << score << endl;
			std::cout << "Press 'Space' to try again or 'Esc' to exit" << endl << endl << endl;
		}
	}
	
	//Desenarea pasarii
	{	
		//Verificari suplimentare pentru incadrarea pasarii in interiorul ecranului
		if (birdPosition > screenOffset)
			birdPosition = screenOffset;

		if (birdPosition < -screenOffset)
		{
			birdPosition = -screenOffset;
			birdAcceleration = 0.0f;
			birdVelocity = 0.0f;
		}

		/*
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdCenterX, birdCenterY);
		RenderMesh2D(meshes["debugDisk2"], shaders["VertexColor"], modelMatrix);
		*/

		//Drawing the bird
		//Unghiuri de rotatie pentru diferite elemente ale pasarii
		float headRotationAngle;
		float wingRotationAngle;
		float eyeRotationAngle;

		if (birdVelocity < 0)
		{
			headRotationAngle = -birdVelocity / 300.0f;
			wingRotationAngle = -birdVelocity / 300.0f;
			eyeRotationAngle = -birdVelocity / 310.0f;
		}
		else
		{
			headRotationAngle = -birdVelocity / 450.0f;
			wingRotationAngle = -birdVelocity / 900.0f;
			eyeRotationAngle = -birdVelocity / 450.0f;
		}

		if (birdVelocity > 550) //Unghiurile de rotatie nu pot depasii anumite praguri
		{
			headRotationAngle = -AI_MATH_PI / 2.0f + 0.3f;
			wingRotationAngle = -0.8f;
			eyeRotationAngle = -AI_MATH_PI / 2.0f + 0.3f;
		}

		//Centrul cercului care incadreaza pasarea pentru coliziune
		birdCenterX = birdX;
		birdCenterY = resolution.y / 2.0f + birdY;

		//Desenarea aripii
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdCenterX - wing_transaltionOffset, resolution.y / 2.0f + birdPosition);
		modelMatrix *= Transform2D::Rotate(wingRotationAngle);
		modelMatrix *= Transform2D::Translate(-birdCenterX + wing_transaltionOffset, -(resolution.y / 2.0f + birdPosition));
		modelMatrix *= Transform2D::Translate(birdCenterX + wing_XOffset, birdCenterY + wing_YOffset);
		modelMatrix *= Transform2D::Scale(wing_XScalingFactor, wing_YScalingFactor);

		//Verificarea orientarii aripii
		if (birdVelocity > 0)
			//Wing is up - bird is falling
			modelMatrix *= Transform2D::Rotate(-AI_MATH_PI * 3.0f / 4.0f);
		else
			//Wing is down - bird is rising
			modelMatrix *= Transform2D::Rotate(AI_MATH_PI * 3.0f / 4.0f);

		modelMatrix *= Transform2D::Translate(-25, -25);
		RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

		//In caz de Game Over pasarea nu mai are pupila
		if (!gameOver)
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(birdCenterX, resolution.y / 2.0f + birdPosition);
			modelMatrix *= Transform2D::Rotate(eyeRotationAngle);
			modelMatrix *= Transform2D::Translate(-birdCenterX, -(resolution.y / 2.0f + birdPosition));
			modelMatrix *= Transform2D::Translate(birdCenterX + eye_XOffset + 3.0f, birdCenterY + eye_YOffset);
			RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);
		}
		
		//Desenarea ochiului pasarii
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdCenterX, resolution.y / 2.0f + birdPosition);
		modelMatrix *= Transform2D::Rotate(eyeRotationAngle);
		modelMatrix *= Transform2D::Translate(-birdCenterX, -(resolution.y / 2.0f + birdPosition));
		modelMatrix *= Transform2D::Translate(birdCenterX + eye_XOffset, birdCenterY + eye_YOffset);
		modelMatrix *= Transform2D::Scale(0.2f, 0.2f);
		RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

		//Desenarea capului pasarii
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdCenterX, resolution.y / 2.0f + birdPosition);
		modelMatrix *= Transform2D::Rotate(headRotationAngle);
		modelMatrix *= Transform2D::Translate(-birdCenterX, -(resolution.y / 2.0f + birdPosition));
		modelMatrix *= Transform2D::Translate(birdCenterX + birdHead_XOffset, birdCenterY + birdHead_YOffset);
		RenderMesh2D(meshes["birdHead"], shaders["VertexColor"], modelMatrix);

		//Desenarea corpului pasarii
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, birdPosition);
		RenderMesh2D(meshes["birdBody"], shaders["VertexColor"], modelMatrix);
		birdY = birdPosition;

		//Desenarea ciocului pasarii
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdCenterX, resolution.y / 2.0f + birdPosition);
		modelMatrix *= Transform2D::Rotate(headRotationAngle);
		modelMatrix *= Transform2D::Translate(-birdCenterX, -(resolution.y / 2.0f + birdPosition));
		modelMatrix *= Transform2D::Translate(birdCenterX + beak_XOffset, birdCenterY + beak_YOffset);
		RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
	}
	//End of drawing the bird

	for (int i = 0; i < nrOfPipes; i++)
	{	
		//Verificarea coliziunii dupa desenarea pasarii
		if (Collision())
		{	
			startGame = false;
			gameOver = true;
			if (displayedGameOver == false)
			{
				displayedGameOver = true;
				std::cout << "		Game Over" << endl;
				std::cout << "Attempts: " << nrOfAttempts << endl << endl;
				std::cout << "You final SCORE is: " << score << endl;
				std::cout << "Press 'Space' to try again or 'Esc' to exit" << endl << endl << endl;
			}
		}

		//Only for debugging (Bottom)
		/*
		float pipeRadius = pipeWidth / 2.0f;
		for (float bottomCenterY = 0; bottomCenterY < (float)pipeWidth * pipes[i].y - pipeRadius; bottomCenterY += pipeRadius / 2.0f)
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(pipes[i].x + pipeRadius, bottomCenterY);
			RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);
		}

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth + pipeRadius, pipeWidth * pipes[i].y - pipeRadius);
		RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth + pipeWidth, pipeWidth * pipes[i].y - pipeRadius);
		RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);
		*/
		//End of Bottom Debugging


		//Bottom pipe end
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth, pipeWidth * pipes[i].y - pipeWidth * pipeEnd_scalingFactorY);
		modelMatrix *= Transform2D::Scale(pipeEnd_scalingFactorX, pipeEnd_scalingFactorY);
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);

		//Bottom pipe
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x, 0);
		modelMatrix *= Transform2D::Scale(1, pipes[i].y);
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);

		//Only for debugging (Top)
		/*
		for (float topCenterY = pipeWidth * pipes[i].y + pipes[i].z + pipeRadius + 5.0f; topCenterY < resolution.y + pipeWidth; topCenterY += pipeRadius / 2.0f)
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate((pipes[i].x + pipeRadius), topCenterY);
			RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);
		}

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth + pipeRadius, pipeWidth * pipes[i].y + pipes[i].z + pipeRadius);
		RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth + pipeWidth, pipeWidth * pipes[i].y + pipes[i].z + pipeRadius);
		RenderMesh2D(meshes["debugDisk"], shaders["VertexColor"], modelMatrix);
		*/
		//End of Top Debugging

		//Top pipe end
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(pipes[i].x - 0.25f * pipeWidth, pipeWidth * pipes[i].y + pipes[i].z);
		modelMatrix *= Transform2D::Scale(pipeEnd_scalingFactorX, pipeEnd_scalingFactorY);
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);

		//Top pipe
		modelMatrix = glm::mat3(1);
		float scalingFactor = (resolution.y - pipes[i].y * pipeWidth - pipes[i].z - pipeWidth);
		modelMatrix *= Transform2D::Translate(pipes[i].x, pipeWidth * pipes[i].y + pipes[i].z);
		modelMatrix *= Transform2D::Scale(1, scalingFactor);
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);
	}

	//Draw the background
	modelMatrix = glm::mat3(1);
	if(day)
		RenderMesh2D(meshes["bgFrontDay"], shaders["VertexColor"], modelMatrix);
	else if(noon)
		RenderMesh2D(meshes["bgFrontNoon"], shaders["VertexColor"], modelMatrix);
	else
		RenderMesh2D(meshes["bgFrontNight"], shaders["VertexColor"], modelMatrix);

	//Front Hills
	for (int i = 0; i < nrOfHills; i++)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(hills[i].x, frontHills_YOffset * hills[i].y);
		modelMatrix *= Transform2D::Scale(hills[i].z, hills[i].z);
		if (day)
			RenderMesh2D(meshes["frontHillsDay"], shaders["VertexColor"], modelMatrix);
		else if (noon)
			RenderMesh2D(meshes["frontHillsNoon"], shaders["VertexColor"], modelMatrix);
		else
			RenderMesh2D(meshes["frontHillsNight"], shaders["VertexColor"], modelMatrix);
	}

	//Draw the Background pipes
	for (int i = 0; i < nrOfBgPipes; i++)
	{
		//Bottom Background pipe end
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bgPipes[i].x - 0.25f * bgPipeWidth, bgPipeWidth * bgPipes[i].y - bgPipeWidth * pipeEnd_scalingFactorY);
		modelMatrix *= Transform2D::Scale(pipeEnd_scalingFactorX, pipeEnd_scalingFactorY);
		if(day)
			RenderMesh2D(meshes["bgPipeDay"], shaders["VertexColor"], modelMatrix);
		else if(noon)
			RenderMesh2D(meshes["bgPipeNoon"], shaders["VertexColor"], modelMatrix);
		else if(night)
			RenderMesh2D(meshes["bgPipeNight"], shaders["VertexColor"], modelMatrix);
		

		//Bottom Background pipe
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bgPipes[i].x, 0);
		modelMatrix *= Transform2D::Scale(1, bgPipes[i].y);
		if (day)
			RenderMesh2D(meshes["bgPipeDay"], shaders["VertexColor"], modelMatrix);
		else if (noon)
			RenderMesh2D(meshes["bgPipeNoon"], shaders["VertexColor"], modelMatrix);
		else if (night)
			RenderMesh2D(meshes["bgPipeNight"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = glm::mat3(1);
	if (day)
		RenderMesh2D(meshes["bgBackDay"], shaders["VertexColor"], modelMatrix);
	else if (noon)
		RenderMesh2D(meshes["bgBackNoon"], shaders["VertexColor"], modelMatrix);
	else
		RenderMesh2D(meshes["bgBackNight"], shaders["VertexColor"], modelMatrix);

	//Back Hills
	for (int i = 0; i < nrOfHills; i++)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(hillsBack[i].x, backHills_YOffset * hillsBack[i].y);
		modelMatrix *= Transform2D::Scale(hillsBack[i].z, hillsBack[i].z);
		if (day)
			RenderMesh2D(meshes["backHillsDay"], shaders["VertexColor"], modelMatrix);
		else if (noon)
			RenderMesh2D(meshes["backHillsNoon"], shaders["VertexColor"], modelMatrix);
		else
			RenderMesh2D(meshes["backHillsNight"], shaders["VertexColor"], modelMatrix);
	}
	
	//Stars
	if (night)
	{	
		for (int i = 0; i < nrOfStars; i++)
		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(stars[i].x, stars[i].y);
			modelMatrix *= Transform2D::Scale(stars[i].z, stars[i].z);
			RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
		}
	}

	//Draw Clouds
	if (!night)
	{
		for (int i = 0; i < nrOfClouds; i++)
		{
			if (clouds[i].z != 0)
			{
				if (clouds[i].z <= 2)
				{
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[4].x, clouds[i].y + cloudsOffsets[4].y);
					modelMatrix *= Transform2D::Scale(1.3f, 1.3f);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);


					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[5].x, clouds[i].y + cloudsOffsets[5].y);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);


					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x, clouds[i].y);
					modelMatrix *= Transform2D::Scale(0.75f, 0.75f);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
				}
				else
				{
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[0].x, clouds[i].y + cloudsOffsets[0].y);
					modelMatrix *= Transform2D::Scale(2.0f, 2.0f);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[1].x, clouds[i].y + cloudsOffsets[1].y);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[2].x, clouds[i].y + cloudsOffsets[2].y);
					modelMatrix *= Transform2D::Scale(1.15f, 1.15f);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x + cloudsOffsets[3].x, clouds[i].y + cloudsOffsets[3].y);
					modelMatrix *= Transform2D::Scale(1.4f, 1.4f);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);

					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(clouds[i].x, clouds[i].y);
					RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
				}	
			}
		}
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{	
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	
	//Tratarea cazului in care pasarea incearca sa paraseasca ecranul prin marginea inferioara
	if (window->KeyHold(GLFW_KEY_SPACE) && birdPosition <= -screenOffset)
	{	
		birdPosition = -screenOffset + 1.0f;
		birdAcceleration = 0.0f;
		birdVelocity = (-gravity) / 8.0f;
	}

	//Utilizatorul apasa 'SPACE' => pasarea trebuie sa sara
	if (window->KeyHold(GLFW_KEY_SPACE) && birdVelocity >= gravity / 300.0f)
	{	
		birdAcceleration = 0.0f;
		birdVelocity = (-gravity) / 8.0f;
	}

	//UnPause the game
	if (window->KeyHold(GLFW_KEY_SPACE) && startGame == false)
		startGame = true;

	//Pause the game
	if (window->KeyHold(GLFW_KEY_P) && startGame == true && gameOver == false)
	{
		startGame = false;
		std::cout << endl << "You just paused the game." << endl;
		std::cout << "To resume just press 'SPACE'" << endl;
	}

	//Restart the game after Game Over
	if (window->KeyHold(GLFW_KEY_SPACE) && gameOver == true)
	{	
		InitConstants();
		InitPipes();
		InitBird();
		InitClouds();
		InitBackground();
		InitBackgroundPipes();
		InitConstants();
		nrOfAttempts++;
		gameOver = false;
		displayedGameOver = false;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}