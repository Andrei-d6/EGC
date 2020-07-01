#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"
#include <vector>

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void RenderSimpleMesh2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void RenderSimpleMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	float getMouseWorldPos(); //Coreleaza pozitia mouse-ului (coordonata Y) in coordonatele scenei
	bool  collisionWithPoint(float x, float y, float z, float radius); //Verificarea coliziunii ssferelor ce incadreaza avionul
																	   //cu un punct dat (centrul sferei pentru care se verifica suprapunerea)

	void InitPlaneBody(); //Initializarea datelor ce descriu corpul avionului
	void InitCross(); //Initializarea sectiunii de pe corpul avionului
	void InitPlaneFront(); //Initializarea datelor ce descriu partea din fata a avionului
	void InitPlanePropeller(); //Initializarea datelor ce descriu elicea avionului
	void InitPlaneWing(); //Initializarea datelor ce descriu aripile avionului
	void InitWingSupports(); //Initializarea suportilor pentru aripile avionului
	void InitPlaneTailSupport(); //Initializarea suportului pentru coada avionului
	void InitPlaneTail(); //Initializarea cozii avionului
	void InitWheels(); //Initializarea datelor care descriu rotile avionului
	void InitPlane(); //Initializarea datelor ce descriu avionul

	void  drawPropeller(); //Desenarea elicei avionului
	void  drawPlanePart(const char* name); //Desenarea unei parti stationare a avionului (nu elicea)
	void  drawFallingPlanePart(const char* name, float rotation); //Desenarea avionului pentru animatia de final
	void  drawPlaneHitBox(); //Desenarea sferelor ce incadreaza avionul

	void InitSky(); //Initializarea sky-boxului - cubul ce incadreaza scena
	void InitClouds(); //Initializarea datelor ce descriu norii
	void drawCloud(int index); //Desenarea norilor

	void InitObstacle(); //Initializarea formei care descrie obstacolul
	void InitObstacleList(); //Initializarea listei de obstacole ce vor fi redate in scena 
	bool ObstacleCollision(); //Verificarea coliziunii avionului cu obstacole
	bool goodObstacleY(int index); //Verificarea pentru incadrarea unui obstacol in spatiul vizibil al scenei
									//gasirea unei valori potrivite pentru coordonata Y a unui obstacol
	bool isObstacleVisible(int index); //Verificarea vizibilitatii unui obstacol
										//Un obstacol este invizibil in urma unei coliziuni sau daca acesta nu a avut sansa de a aparea in scena

	void InitFuel(); //Initializarea unei unitati de combustibil
	void InitFuelBar(); //Initializarea barei de combustibil
	void InitFuelList(); //Initializarea listei de untitati de combustibil
	bool isFuelVisible(int index); //Verificarea vizibilitatii unei unitati de combustibil
									//o unitate de combustibil este invizibila in urma unei coliziunii a avionului cu aceasta
	void FuelCollision(); //Verificarea coliziunii avionului cu unitatile de combustibil

	void  drawHP(); //Desenarea inimilor reprezentand viata jucatorului

protected:
	//General view parameters
	Laborator::MyCamera* firstPersonCamera; //First Person Camera - mobila
	Laborator::MyCamera* thirdPersonCamera; //Third Person Camera - stationara
	glm::mat4 projectionMatrix; //matricea de proiectie
	GLenum polygonMode; //Modul de afisarea al poligoanelor
	glm::ivec2 resolution; //Rezolutia ecranului
	float screenOffset; //Offset-ul pana la marginea ecranului (in coordonate locale)

	glm::vec3 lightPosition; //pozitia sursei de lumina
	//parametrii de material - variaza pentru elementele scenei
	unsigned int materialShininess; 
	float materialKd;
	float materialKs;
	
	//Mouse parameters
	float mouse; //Coordonata Y in spatiul scenei a mouse-ului
	float cursorY; //Coordonata Y absoluta a cursorului
	float cursorX; //Coordonata X absoluta a cursorului

	int  wait; //Semafor pentru asteptarea de la finalul animatiei intiale
	int  waitTime; //Contor pentru timpul de afisare dupa animatia initiala
	bool debugMode; //Activarea sau Dezactivarea coliziunilor cu obstacole 
	bool firstPerson  = false; //Semafor pentru modul de afisare first person
	bool thirdPerson  = true; //Semafor pentru modul de afisare third person (implicit)
	bool switchCamera = false; //Semafor pentru folosirea unei camere in interiorul scenei
	
	//Plane parameters
	glm::vec3 planeCoordinates; //Coordonatele avionului (x, y, z)
	float planeX, planeY; //Coordonatele initiale ale avionului (la finalul animatiei initiale)
	float newPlaneX; //Coordonata X a avionului in urma unei coliziuni

	bool rotatePlane  = true; //Semafor pentru rotatia avionului
	float planeRotationFactor = 0.0f; //Gradul de rotatie al avionului
	float planeScalingFactor  = 0.4f; //Factorul de scalare al modelului avionului
	float fallingRotationFactor; //Gradul de rotatie al avionului pentru animatia finala

	glm::vec3 planeFrontBottomRight; //Coltul dreapta jos al varfului avionului
	glm::vec3 planeFrontTopRight; //Coltul dreapata sus al varfului avionului

	//Propeller parameters
	float planePropellerWidth; //Latimea elicei avionului
	float propellerXOffset; //Offset-ul pe axa X pentru alicea avionului
	float propellerZOffset; //Offset-ul pe axa Y pentru elicea avionului
	float propellerYOffset; //Offset-ul pe axa Z pentru elicea avionului
	glm::vec2 propellerCenter; //Centrul elicei avionului
	float propellerRotationFactor; //Factorul de rotatie a elicei avionului
	
	//General game parameters
	bool gameOver; //Semafor pentru marcarea finalului unui joc (Game Over)
	bool initialAnimation = true; //Semafor pentru animatia de inceput de joc a avionului
	bool finalAnimation   = false; //Semafor pentru animatia de final de joc a avionului
	bool hit; //Semafor pentru marcarea unei coliziuni a avionului cu un obstacol
	bool comingBack; //Semafor pentru marcarea intoarcerii avionului catre coordonatele de baza (planeX, planeY)

	//Scene elements parameters
	//Obstacle parameters
	int nrOfObstacles; //Numarul maxim de obstacole posibile in scena
	int obstacleCounter; //Contor pentru marcarea trecerii unui numar de liste de obstacole
	float maxObstacleY; //Coordonata Y maxima a unui obstacol
	float minObstacleY; //Coordonata Y minima a unui obstacol
	float attempts; //Numarul de incercari posibile pentru initializarea coordonatelor unui nou obstacol
	
	//Fuel parameters
	int   nrOfFuel; //Numarul de unitati de combustibil dintr-un sir generat
	float fuelSpeed = 0.8; //Viteza de deplasare a unitatilor de combustibil
	float fuelBarScalingFactor = 100.0f; //Factorul de scalare pentru indicarea nivelului de combustibil ramas: 100->plin   0->gol
	float fuelDistance; //Distanta dintre doua unitati de combustibil
	float maxFuelY; //Coordonata Y maxima a unei unitati de combustibil
	float minFuelY; //Coordonata Y minima a unei unitati de combustibil

	int nrOfClouds; //Numarul maxim de nori din scena
	float waterRotationFactor; //Factorul de rotatie al apei 
	int currentOption; //Tipul de apa afisat la momentul actual
	
	int HP; //Viata jucatorului: 3->plin 0->gol
	float heartRotationFactor; //Factorul de rotatie al inimilor desenate pe ecran
};
