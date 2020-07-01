#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

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


	void InitPipes();
	void InitConstants();
	void InitBird();
	void InitClouds();
	void InitBackground();
	void InitStars();
	void InitBackgroundPipes();
	bool Collision();

protected:
	glm::mat3 modelMatrix;
	glm::ivec2 resolution;

	//Bird parameters
	float birdX, birdY; //Coordonatele pasarii
	float birdCenterX, birdCenterY; //Coordonatele centrului pasarii

	float birdPosition; //Deplasarea pasarii fata de pozitia initiala
	float birdVelocity; //Viteza pasarii
	float birdAcceleration; //Acceleratia pasarii

	float birdRadius, birdBodyRadius, birdHeadRadius, birdEyeRadius; //Raza pasarii, a corpului, a capului si a ochiului
	
	float birdHead_XOffset, birdHead_YOffset; //Deplasarea pozitiei capului fata de centrul pasarii
	float beak_XOffset, beak_YOffset, beakLength; //Deplasarea pozitiei ciocului fata de centrul pasarii 
												  // si lungimea ciocului
	float eye_XOffset, eye_YOffset; //Deplaasarea pozitiei ochiului fata de centrul pasarii
	float wing_XOffset, wing_YOffset; //Deplasarea aripii fata de centrul pasarii
	float wing_XScalingFactor, wing_YScalingFactor, wing_transaltionOffset; //Coeficientii de scalare ai aripii

	glm::vec3 birdBodyRGB, birdHeadRGB, birdBeakRGB; //Culoarea pentru cord, cap si cioc a pasarii



	//Pipe parameters
	int nrOfPipes; //Numarul de pipe-uri din ecran la un anumit moment de timp
	int pipeWidth; //Latimea unui pipe
	float minGapHeight; //Spatiul minim dintre o pereche de pipe-uri
	float maxGapHeight; //Spatiul maxim dintre o pereche de pipe-uri
	float pipeDistance, pipeSpeed; //Distanta dintre doua pipe-uri; viteza de deplasare pe axa Ox
	
	float pipeRadius; //Raza cercurilor incadrate in interiorul pipe-urilor (pentru coliziuni)
	float pipeEnd_scalingFactorX, pipeEnd_scalingFactorY; //Coeficienti de scalare pentru capetele pipe-urilor
	bool firstPipe; //Semafor pentru trecerea prin primul pipe pentru ajustarea scorului

	int nrOfBgPipes; //Numarul de pipe-uri din fundal
	float bgPipesSpeed; //Viteza de translatie pe Ox a pipe-urilor din fundal
	float bgPipeWidth; //Latimea pipe-urilor din fundal
	float bgPipeDistance; //Distanta dintre doua pipe-uri din fundal
	float maxSlidingFactor; //Gradul maxim de translatie pe verticala al pipe-urilor (viteza de translatie)
	
	glm::vec3 bgPipeDayRGB, bgPipeNoonRGB, bgPipeNightRGB; //Culorile pentru pipe-urile de fundal pentru zi, seara si noapte



	//Cloud parameters
	int nrOfClouds; //Numarul maxim de nori pe ecran la un anumit moment de timp
	float cloudRadius; //Raza primului nor
	float cloudSpeed; //Viteza de deplasare a norilor
	


	//Background parameters
	int nrOfHills; //Numarul de dealuri din fundal
	float backgroundLength, backgroundWidth; //Latimea dreptunghiurilor din fundal de la baza dealurilor
	
	float hillRadius; //Raza standard a unui cerc din lantul de dealuri
	float hillSpeed, hillBackSpeed; //Vitezele de translatie pe axa Ox a dealurilor din fata si din spate

	glm::vec3 frontHillsRGBDay, frontHillsRGBNight, frontHillsRGBNoon; //Culorile dealurilor din fata
	glm::vec3 backHillsRGBDay, backHillsRGBNight, backHillsRGBNoon; //Culorile dealurilor din spate
	
	float frontHills_YOffset; //Offset-ul pe Oy pentru dealurile din fata
	float backHills_YOffset; //Offest-ul pe Oy pentru dealurile din spate



	//Sky parameters
	glm::vec3 skyDay, skyNight, skyNoon; //Culorile cerului pentru zi, seara si noapte
	glm::vec3 skyRGB; //Culoarea finala a cerului
	float nrOfStars; //Numarul de stele de pe timpul noptii
	float starsSpeed; //Viteza de translatie a stelelor


	int nrOfAttempts = 1; //Numarul de incercari
	int score; //Scorul din cadrul jocului curent
	float gravity; //Acceleratia gravitationala a lumii jocului
	float screenOffset; //Deplasarea maxima a pasarii fata de marginile ecranului

	bool startGame = false; //Semafor pentru marcarea inceputului de joc
	bool gameOver = false; //Semafor pentru marcarea finalului unui joc (Game Over)
	bool displayedGameOver = false; //Semafor pentru verificarea afisarii mesajului de "Game Over"

	int timeCounter; //Contor pentru tranzitie intre momentele zilei
	int timeCap; //Contor pentru cresterea vitezei de translatie verticale a pipe-urilor
	bool day, noon, night; //Semafoare pentru indicarea momentului curent al zilei
};
