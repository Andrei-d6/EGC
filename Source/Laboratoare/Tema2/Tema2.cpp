#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Object3D.h"
#include "../Laborator4/Transform3D.h"

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

vector <glm::vec3> obstacles; //Lista coordonatelor obstacolelor
vector <glm::vec3> obstaclesAux; //Informatie suplimentara pentru obstacole (coliziune, sansa de aparitie, grad de rotatie)
vector <glm::vec4> obstaclesPos; //Pozitia actuala a obstacolelor (in timpul rotirii)
vector <float>obstaclesSpeed; //Vitezele de deplasare ale obstacolelor

vector <glm::vec3> fuel; //Lista coordonatelor unitatilor de combustibil
vector <glm::vec3> fuelAux; //Informatii auxiliare pentru unitatile de combustibil (vizibilitate, grad de rotatie)
vector <glm::vec4> fuelPos; //Pozitia actuala a unitatilor de combustibil (in timpul rotirii)

vector <glm::vec3> clouds3D; //Lista coordonatelor norilor
vector <glm::vec3> cloudsAux; //Informatie auxiliara pentru descrierea norilor (sansa de aparitie, tipul norilor)
vector <glm::vec4> cloudsPos;  //Pozitia actuala a norilor (in timpul rotirii)

bool fallingWings = true;

int score = 0;

void Tema2::InitPlaneBody()
{
	planeRotationFactor = 0.0f;
	float R, G, B;
	R = 190 / 255.0f; G = 42 / 255.0f; B = 23 / 255.0f;

	vector<VertexFormat> vertices
	{
		//The face of the body
		VertexFormat(glm::vec3(-7, -0.5f,  0.5f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //Bottom left 
		VertexFormat(glm::vec3(1.5f, -1.5f,  1.5f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-7, 0.5f,  0.5f), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(1.5f,  2,  1.5f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

		//The back of the body
		VertexFormat(glm::vec3(-7, -0.5f, -0.5f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left
		VertexFormat(glm::vec3(1.5f, -1.5f, -1.5f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-7,  0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(1.5f,  2, -1.5f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};
	meshes["planeBody"] = Object3D::CreateMesh("planeBody", vertices, indices);
}

void f(float xleft, float xright)
{	//Functie pentru calcularea unor coordonate pe corpul avionului date fiind
	//expressile functiilor care definesc conturul corpului avionului
	float yleftBottom, yrightBottom;
	yleftBottom = -0.117647f * xleft - 1.3235f;
	yrightBottom = -0.117647f * xright - 1.3235f;
	//std::cout << "Bottom: " << yleftBottom << " " << yrightBottom << endl;


	float yleftTop, yrightTop;
	yleftTop = 0.1764f * xleft + 1.7352f;
	yrightTop = 0.1764f * xright + 1.7352f;;
	//std::cout << "Top: " << yleftTop << " " << yrightTop << endl;


	float zleft, zright;
	zleft = 0.2352f * xleft + 2.647f;
	zright = 0.2352f * xright + 2.647f;
	//std::cout << "Z: " << zleft /2.0f << " " << zright /2.0f << endl;
}

void Tema2::InitCross()
{
	float R, G, B;
	R = 255 / 255.0f; G = 255 / 255.0f; B = 255 / 255.0f;

	f(-5.5f, -3.5f);

	vector<VertexFormat> vertices
	{
		//The face of the body
		VertexFormat(glm::vec3(-5.5f, -0.69f,  0.685f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //Bottom left 
		VertexFormat(glm::vec3(-3.5f, -0.93f,  0.925f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-5.5f, 0.78f,  0.685f), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(-3.5f,  1.13f,  0.925f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

		//The back of the body
		VertexFormat(glm::vec3(-5.5f, -0.69f, -0.685f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left
		VertexFormat(glm::vec3(-3.5f, -0.93f, -0.925f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-5.5f,  0.78f, -0.685f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(-3.5f,  1.13f, -0.925f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};
	meshes["planeCrossBG"] = Object3D::CreateMesh("planeCrossBG", vertices, indices);
}

void Tema2::InitPlaneFront()
{
	//Variabile folosite pentru determinarea pozitiei elicei
	planeFrontBottomRight = glm::vec3(3, -0.5f, 1.0f);
	planeFrontTopRight = glm::vec3(3, 1.5f, 1.0f);

	vector<VertexFormat> vertices
	{
		//The face of the front
		VertexFormat(glm::vec3(1.5f, -1.5f,  1.5f), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)), //Bottom left 
		VertexFormat(glm::vec3(3, -0.5f,  1.0f), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1)),  //Bottom right
		VertexFormat(glm::vec3(1.5f, 2,  1.5f), glm::vec3(1, 0, 0), glm::vec3(1, 1, 1)), //Top left
		VertexFormat(glm::vec3(3,  1.5f,  1.0f), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)),  //Top right

		//The back of the front
		VertexFormat(glm::vec3(1.5f, -1.5f, -1.5f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)), //Bottom left
		VertexFormat(glm::vec3(3, -0.5f, -1.0f), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)),  //Bottom right
		VertexFormat(glm::vec3(1.5f,  2, -1.5f), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)), //Top left
		VertexFormat(glm::vec3(3,  1.5f, -1.0f), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};
	meshes["planeFront"] = Object3D::CreateMesh("planeFront", vertices, indices);
}

void Tema2::InitPlanePropeller()
{
	planePropellerWidth = 0.2f;
	propellerXOffset = planePropellerWidth - 0.1f;
	propellerZOffset = 0.2f;
	propellerYOffset = 0.2f;
	propellerCenter = glm::vec2(planeFrontBottomRight.x * planeScalingFactor, (planeFrontBottomRight.y * planeScalingFactor + planeFrontTopRight.y * planeScalingFactor) / 2.0f);
	propellerRotationFactor = 0;

	vector<VertexFormat> vertices
	{
		//The face of the body
		VertexFormat(glm::vec3(0, -0.1f,  0.1f), glm::vec3(0, 1, 1), glm::vec3(0, 0, 0)), //Bottom left
		VertexFormat(glm::vec3(3, -0.5f,  0.1f), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0)),  //Bottom right
		VertexFormat(glm::vec3(0, 0.1f,  0.1f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)), //Top left
		VertexFormat(glm::vec3(3,  0.5f,  0.1f), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0)),  //Top right

		//The back of the body
		VertexFormat(glm::vec3(0, -0.1f, -0.1f), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)), //Bottom left
		VertexFormat(glm::vec3(3, -0.5f, -0.1f), glm::vec3(0, 1, 1), glm::vec3(0, 0, 0)),  //Bottom right
		VertexFormat(glm::vec3(0,  0.1f, -0.1f), glm::vec3(1, 1, 0), glm::vec3(0, 0, 0)), //Top left
		VertexFormat(glm::vec3(3,  0.5f, -0.1f), glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

	meshes["planePropeller"] = Object3D::CreateMesh("planePropeller", vertices, indices);
}

void Tema2::InitPlaneWing()
{
	float R, G, B;
	R = 170 / 255.0f; G = 23 / 255.0f; B = 23 / 255.0f;

	vector<VertexFormat> vertices
	{
		//The face of the wing
		VertexFormat(glm::vec3(-3, -0.3f,  3.7f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //Bottom left 
		VertexFormat(glm::vec3(1, -0.3f, 3.7f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-3, 0.3f, 3.7f), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(1, 0.3f, 3.7f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

		//The back of the wing
		VertexFormat(glm::vec3(-3, -0.3f, -3.7f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left
		VertexFormat(glm::vec3(1, -0.3f, -3.7f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-3, 0.3f, -3.7f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(1, 0.3f, -3.7f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};
	meshes["planeWing"] = Object3D::CreateMesh("planeWing", vertices, indices);
}

void Tema2::InitWingSupports()
{
	{
		float R, G, B;
		R = 180 / 255.0f; G = 23 / 255.0f; B = 23 / 255.0f;

		vector<VertexFormat> vertices
		{
			//The face of the wing
			VertexFormat(glm::vec3(-1.1f, -0.3f,  3.5f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //Bottom left 
			VertexFormat(glm::vec3(-0.9f, -0.3f, 3.5f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
			VertexFormat(glm::vec3(-1.1f, 3.7f, 3.5f), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
			VertexFormat(glm::vec3(-0.9f, 3.7f, 3.5f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

			//The back of the wing
			VertexFormat(glm::vec3(-1.1f, -0.3f, 3.7f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left
			VertexFormat(glm::vec3(-0.9f, -0.3f, 3.7f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
			VertexFormat(glm::vec3(-1.1f, 3.7f, 3.7f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
			VertexFormat(glm::vec3(-0.9f,  3.7f, 3.7f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};
		meshes["planeWingSupport"] = Object3D::CreateMesh("planeWingSupport", vertices, indices);
	}

	{
		float R, G, B;
		R = 180 / 255.0f; G = 23 / 255.0f; B = 23 / 255.0f;

		vector<VertexFormat> vertices
		{
			//The face of the wing
			VertexFormat(glm::vec3(-1.1f, -0.3f,  3.5f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //Bottom left 
			VertexFormat(glm::vec3(-0.9f, -0.3f, 3.5f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
			VertexFormat(glm::vec3(-1.1f, 1.9f, 3.5f), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
			VertexFormat(glm::vec3(-0.9f, 1.9f, 3.5f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

			//The back of the wing
			VertexFormat(glm::vec3(-1.1f, -0.3f, 3.7f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left
			VertexFormat(glm::vec3(-0.9f, -0.3f, 3.7f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
			VertexFormat(glm::vec3(-1.1f, 1.9f, 3.7f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
			VertexFormat(glm::vec3(-0.9f, 1.9f, 3.7f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};
		meshes["planeWingSupportSmall"] = Object3D::CreateMesh("planeWingSupportSmall", vertices, indices);
	}

}

void Tema2::InitPlaneTailSupport()
{
	float R, G, B;
	R = 190 / 255.0f; G = 42 / 255.0f; B = 23 / 255.0f;

	vector<VertexFormat> vertices
	{
		//The face of the tail support
		VertexFormat(glm::vec3(-10, 0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(-7, -0.5f, 0.5f), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-7, 0.5f,  0.5f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //Top right

		//The back of the tail support
		VertexFormat(glm::vec3(-10,  0.5f, 0), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //Top left
		VertexFormat(glm::vec3(-7, -0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //Bottom right
		VertexFormat(glm::vec3(-7,  0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //Top right
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
		0, 2, 5,        3, 2, 5,
		0, 4, 5,        3, 4, 5
	};
	meshes["planeTailSupport"] = Object3D::CreateMesh("planeTailSupport", vertices, indices);
}

void Tema2::InitPlaneTail()
{
	float R, G, B;
	R = 190 / 255.0f; G = 42 / 255.0f; B = 23 / 255.0f;

	vector<VertexFormat> vertices
	{
		//Front Face
		VertexFormat(glm::vec3(-10, 0.5f,  0), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //0
		VertexFormat(glm::vec3(-10.5f, 1.5f,  0), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //1
		VertexFormat(glm::vec3(-10.5f, 2,  0), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //2
		VertexFormat(glm::vec3(-10,  3,  0.1f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //3
		VertexFormat(glm::vec3(-8, 3, 0.1f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //4
		VertexFormat(glm::vec3(-7, 1.5f, 0.25f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //5
		VertexFormat(glm::vec3(-7,  1.5f, 0.25f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //6
		VertexFormat(glm::vec3(-7,  0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //7

		//Back Face
		VertexFormat(glm::vec3(-10, 0.5f,  0), glm::vec3(0, 1, 1), glm::vec3(R, G, B)), //8
		VertexFormat(glm::vec3(-10.5f, 1.5f,  0), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),  //9
		VertexFormat(glm::vec3(-10.5f, 2,  0), glm::vec3(1, 0, 0), glm::vec3(R, G, B)), //10
		VertexFormat(glm::vec3(-10,  3,  -0.1f), glm::vec3(0, 1, 0), glm::vec3(R, G, B)),  //11
		VertexFormat(glm::vec3(-8, 3, -0.1f), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //12
		VertexFormat(glm::vec3(-7, 1.5f, -0.25f), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),  //13
		VertexFormat(glm::vec3(-7,  1.5f, -0.25f), glm::vec3(1, 1, 0), glm::vec3(R, G, B)), //14
		VertexFormat(glm::vec3(-7,  0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(R, G, B)),  //15
	};

	vector<unsigned short> indices =
	{
		0, 2, 1,    0, 3, 2,    0, 7, 3,
		3, 7, 4,    4, 7, 5,    5, 7, 6,
		8, 9, 10,   10,11,8,    8,11,15,
		11,12,15,   15,12,13,   15,13,14,
		14, 7,15,   6, 7, 14,   11, 4,12,
		3, 4, 11,   6, 14,13,   5, 6, 14,
		4, 5, 13,   4, 13, 12
	};

	meshes["planeTail"] = Object3D::CreateMesh("planeTail", vertices, indices);
}

void Tema2::InitWheels()
{
	meshes["backWheel"] = Object3D::CreateColoredCylinder("backWheel", 0.8f, 0.2f, 100, 2, false, glm::vec3(0, 0, 0));
	meshes["backWheelCap"] = Object3D::CreateColoredCylinder("backWheelcap", 0.4f, 0.2f, 100, 2, false, glm::vec3(1, 1, 1));

	meshes["frontWheel"] = Object3D::CreateColoredCylinder("frontWheel", 1.2f, 0.2f, 100, 2, false, glm::vec3(0, 0, 0));
	meshes["frontWheelCap"] = Object3D::CreateColoredCylinder("frontWheelCap", 0.7f, 0.4f, 100, 2, false, glm::vec3(1, 1, 1));
}

void Tema2::InitPlane()
{
	planeScalingFactor = 0.4f;
	InitPlaneBody();
	InitPlaneFront();
	InitPlaneTailSupport();
	InitPlaneTail();
	InitCross();
	InitPlanePropeller();
	InitPlaneWing();
	InitWingSupports();
	InitWheels();

	if (!initialAnimation)
		planeCoordinates = glm::vec3(-9, 2, 0);
	else
		planeCoordinates = glm::vec3(-39, -28, 0);

	planeX = -9;
	planeY = 2;
	hit = false;
	fallingRotationFactor = 0;
}



void Tema2::drawPropeller()
{
	//Propeller 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(propellerCenter.x, propellerCenter.y, 0));
		modelMatrix = glm::rotate(modelMatrix, propellerRotationFactor, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-propellerCenter.x, -propellerCenter.y, 0));


		modelMatrix = glm::translate(modelMatrix, glm::vec3((planeFrontBottomRight.x + propellerXOffset) * planeScalingFactor,
			(planeFrontBottomRight.y * planeScalingFactor + planeFrontTopRight.y * planeScalingFactor) / 2.0f,
			propellerZOffset * planeScalingFactor));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes["planePropeller"], shaders["VertexNormal"], modelMatrix);
	}

	//Propeller 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(propellerCenter.x, propellerCenter.y, 0));
		modelMatrix = glm::rotate(modelMatrix, propellerRotationFactor, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-propellerCenter.x, -propellerCenter.y, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3((planeFrontBottomRight.x + propellerXOffset) * planeScalingFactor,
			(planeFrontBottomRight.y * planeScalingFactor + planeFrontTopRight.y * planeScalingFactor) / 2.0f,
			-propellerZOffset * planeScalingFactor));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes["planePropeller"], shaders["VertexNormal"], modelMatrix);
	}

	//Propeller 3
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(propellerCenter.x, propellerCenter.y, 0));
		modelMatrix = glm::rotate(modelMatrix, propellerRotationFactor, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-propellerCenter.x, -propellerCenter.y, 0));


		modelMatrix = glm::translate(modelMatrix, glm::vec3((planeFrontBottomRight.x + propellerXOffset) * planeScalingFactor,
			(planeFrontBottomRight.y * planeScalingFactor + planeFrontTopRight.y * planeScalingFactor) / 2.0f + propellerYOffset * planeScalingFactor,
			0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes["planePropeller"], shaders["VertexNormal"], modelMatrix);
	}

	//Propeller 4
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(propellerCenter.x, propellerCenter.y, 0));
		modelMatrix = glm::rotate(modelMatrix, propellerRotationFactor, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-propellerCenter.x, -propellerCenter.y, 0));


		modelMatrix = glm::translate(modelMatrix, glm::vec3((planeFrontBottomRight.x + propellerXOffset) * planeScalingFactor,
			(planeFrontBottomRight.y * planeScalingFactor + planeFrontTopRight.y * planeScalingFactor) / 2.0f - propellerYOffset * planeScalingFactor,
			0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes["planePropeller"], shaders["VertexNormal"], modelMatrix);
	}
}

void Tema2::drawPlanePart(const char* name)
{
	if (strcmp(name, "planeWing") == 0) {
		//In cazul aripilor desenarea acestora poate depinde de optiunea fallingWings
		//sau de modul de vizualizare - first person sau third person

		//Aripa cea mai de jos este desenata intotdeauna
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

		if (fallingWings)
		{
			//Este selectata optiunea de a pierde aripi pe masura ce avionul se loveste de obstacole

			//Pt HP > 1 (adica maxim o coliziune) se deseneaza si aripile din mijloc
			if (HP > 1 && thirdPerson) {
				modelMatrix = glm::mat4(1);
				modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
				if (rotatePlane)
					modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
				modelMatrix *= Transform3D::Translate(0, 1.5f, 0);
				RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
			}

			//Pt HP > 2 (adica nici o coliziune) se deseneaza si randul superior de aripi
			if (HP > 2 && thirdPerson) {
				modelMatrix = glm::mat4(1);
				modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
				if (rotatePlane)
					modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
				modelMatrix *= Transform3D::Translate(0, 3, 0);
				RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
			}
		}
		else if (thirdPerson)
		{
			//Chiar daca modul falling Wings este dezactivat, randurile 2 si 3 de aripi nu trebuie desenata in first person

			//Desenarea aripilor de la mijloc
			modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			modelMatrix *= Transform3D::Translate(0, 1.5f, 0);
			RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

			//Desenarea randului superior de aripi
			modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			modelMatrix *= Transform3D::Translate(0, 3, 0);
			RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
		}
	}
	else if (strcmp(name, "planeWingSupport") == 0)
	{
		/*
			Suportul pentru aripi este desenat in functie de numarul de aripi desenate
			toate aripile => suport lung
			2 aripi => suport scurt
			o singura aripa => fara suport
		*/
		if (HP == 3 || !fallingWings) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

			modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			modelMatrix *= Transform3D::Translate(0, 0, -7);
			RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
		}
		else if (HP == 2 && fallingWings) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			RenderSimpleMesh2(meshes["planeWingSupportSmall"], shaders["ShaderLight"], modelMatrix);

			modelMatrix = glm::mat4(1);
			modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
			if (rotatePlane)
				modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
			modelMatrix *= Transform3D::Translate(0, 0, -7);
			RenderSimpleMesh2(meshes["planeWingSupportSmall"], shaders["ShaderLight"], modelMatrix);
		}
	}
	else if (strcmp(name, "Wheels") == 0)
	{
		/*
			Desenarea rotilor presupune desenarea mai multor modele:
			2 cilindrii frontali negrii - fronWheel
			2 cilidnrii fronatl albi mai mici - frontWheel Cap

			1 cilindru in spate negru - backWheel
			1 cilindru in spate alb - backWheelCap
		*/

		//Roata frontala 1
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, 1.26f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheelCap"], shaders["ShaderLight"], modelMatrix);


		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, 1.25f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheel"], shaders["ShaderLight"], modelMatrix);

		//Roata frontala 2
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, -1.26f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheelCap"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, -1.27f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheel"], shaders["ShaderLight"], modelMatrix);


		//Roata din spate
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(-7, -1, 0);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["backWheel"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(-7, -1, 0.09f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["backWheelCap"], shaders["ShaderLight"], modelMatrix);
	}
	else {
		//Desenarea oricarei alte parti stationare a avionului
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		if (rotatePlane)
			modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
	}
}

void Tema2::drawFallingPlanePart(const char* name, float rotation)
{
	if (strcmp(name, "Wheels") == 0)
	{
		/*
			Desenarea rotilor in animatia finala de cadere presupune rotirea si desenarea mai multor forme:
			2 cilindrii frontali negrii - fronWheel
			2 cilidnrii fronatl albi mai mici - frontWheel Cap

			1 cilindru in spate negru - backWheel
			1 cilindru in spate alb - backWheelCap
		*/

		//Roata frontala 1
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, 1.26f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheelCap"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, 1.25f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheel"], shaders["ShaderLight"], modelMatrix);


		//Roata frontala 2
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, -1.26f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheelCap"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(1, -2.5, -1.27f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["frontWheel"], shaders["ShaderLight"], modelMatrix);

		//Roata din spate
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(-7, -1, 0);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["backWheel"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(-7, -1, 0.09f);
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		RenderSimpleMesh2(meshes["backWheelCap"], shaders["ShaderLight"], modelMatrix);
	}
	else if (strcmp(name, "planeWing") == 0 && fallingWings == false)
	{
		//Desenarea aripilor in animatia de cadere se face doar pentru fallingWings dezactivat

		//Randul de aripi 2
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(0, 1.5f, 0);
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

		//Randul de aripi 3
		modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(0, 3, 0);
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);

		//Suportul de aripi mai aproape de camera third person
		modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes["planeWingSupport"], shaders["ShaderLight"], modelMatrix);

		//Suportul de aripi indepartat de camera third person
		modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor / 6.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		modelMatrix *= Transform3D::Translate(0, 0, -7);
		RenderSimpleMesh2(meshes["planeWingSupport"], shaders["ShaderLight"], modelMatrix);
	}
	else
	{
		//Desenarea restului de parti stationare ale avionului in animatia de cadere
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationFactor), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotation), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(planeScalingFactor));
		RenderSimpleMesh2(meshes[name], shaders["ShaderLight"], modelMatrix);
	}
}

void Tema2::drawPlaneHitBox()
{
	//Plane HitBox Right
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x, planeCoordinates.y, planeCoordinates.z);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
		RenderSimpleMesh(meshes["planeHitBox"], shaders["VertexNormal"], modelMatrix);
	}

	//Plane HitBox Left
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(planeCoordinates.x - 8 * planeScalingFactor, planeCoordinates.y + 1 * planeScalingFactor, planeCoordinates.z);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
		RenderSimpleMesh(meshes["planeHitBox"], shaders["VertexNormal"], modelMatrix);
	}
}



void Tema2::InitSky()
{
	float R, G, B;
	R = 201 / 255.0f; G = 219 / 255.0f; B = 255 / 255.0f;

	// Create the sky
	{
		vector<VertexFormat> vertices
		{
			//The face of the sky
			VertexFormat(glm::vec3(-170, -170,  172), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),//Bottom left 
			VertexFormat(glm::vec3(170, -170,  172), glm::vec3(1, 0, 1), glm::vec3(R, G, B)),//Bottom right
			VertexFormat(glm::vec3(-170,  170,  172), glm::vec3(1, 0, 0), glm::vec3(R, G, B)),//Top left
			VertexFormat(glm::vec3(170,  170,  172), glm::vec3(0, 1, 0), glm::vec3(R, G, B)), //Top right

			//The back of the sky
			VertexFormat(glm::vec3(-170, -170, -172), glm::vec3(1, 1, 1), glm::vec3(R, G, B)), //Bottom left 
			VertexFormat(glm::vec3(170, -170, -172), glm::vec3(0, 1, 1), glm::vec3(R, G, B)),//Bottom right
			VertexFormat(glm::vec3(-170,  170, -172), glm::vec3(1, 1, 0), glm::vec3(R, G, B)),//Top left
			VertexFormat(glm::vec3(170,  170, -172), glm::vec3(0, 0, 1), glm::vec3(R, G, B)), //Top right
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		meshes["sky"] = Object3D::CreateMesh("sky", vertices, indices);
	}
}

void Tema2::InitClouds()
{
	// Create a simple cube
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, -1,  1),  glm::vec3(1, 0, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1,  1,  1),  glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, -1, -1),  glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1,  1, -1),  glm::vec3(0, 0, 1), glm::vec3(1, 1, 1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};
		meshes["cloud"] = Object3D::CreateMesh("cloud", vertices, indices);
	}

	nrOfClouds = 10;
	clouds3D.resize(nrOfClouds);
	cloudsAux.resize(nrOfClouds);
	cloudsPos.resize(nrOfClouds);

	/*
		Initializarea datelor care descriu norii
		clouds3D[i].x - coordonata X a norului i inainte de rotatie
		clouds3D[i].y - coordonata Y a norului i inainte de rotatie
		clouds3D[i].z - coordonata Z a norului i inainte de rotatie

		Initializarea datelor auxiliare care descriu norii
		cloudsAux[i].x - sansa de aparitie a norului si tipul acestuia (forma)

	*/
	for (int i = 0; i < nrOfClouds; i++)
	{
		cloudsAux[i].x = rand() % 5; //chance to spawn and cloud type

		clouds3D[i].x = 1.5f * screenOffset + (rand() % 4 + 1) * (rand() % 40 + (float)(rand() % 100) / 100);
		attempts = 21;
		do {
			attempts--;
			clouds3D[i].y = rand() % 10 + (float)(rand() % 100) / 100;
		} while (clouds3D[i].y >= 6 || attempts > 0);

		do {
			clouds3D[i].z = -2 * rand() % 10 - (float)(rand() % 100) / 100;
		} while (clouds3D[i].z >= -5);

		//Initializarea coordonatelor in spatiul lumii a norilor in urma rotatiei
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].x), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
		modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
		cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
	}
}

void Tema2::drawCloud(int i)
{
	//Desenarea norilor se face dupa tipul acestora
	//In cazul in care camera este first person, acele coordonatelor sunt schimbate, iar axa fata de care se face rotatia este schimbata
	if (cloudsAux[i].x == 0) {
		//Big center cloud
		glm::mat4 modelMatrix = glm::mat4(1);

		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x / 1.5f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
		}
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);

		//Small left cloud
		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x - 1.8f) / 1.5f, clouds3D[i].y - 0.2f, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x - 1.8f, clouds3D[i].y - 0.2f, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(20), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);

		//Small right cloud
		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x + 1.5f) / 1.5f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x + 1.5f, clouds3D[i].y, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.72f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(40), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);
	}
	else if (cloudsAux[i].x == 1) {
		//Just a smaller cube
		glm::mat4 modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x / 1.2f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));

		cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);
	}
	else if (cloudsAux[i].x == 3) {
		//Big center cloud
		glm::mat4 modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x / 1.3f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);

		//Medium left cloud
		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x - 1.9f) / 1.3f, clouds3D[i].y - 0.2f, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x - 1.9f, clouds3D[i].y - 0.2f, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(15), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);

		//Small left cloud
		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x - 2.9f) / 1.3f, clouds3D[i].y - 0.3f, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x - 2.9f, clouds3D[i].y - 0.3f, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);


		//Small right cloud
		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x + 1.5f) / 1.3f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x + 1.5f, clouds3D[i].y, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.72f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(40), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);
	}
	else if (cloudsAux[i].x == 2) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x / 1.5f, clouds3D[i].y, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(50), glm::vec3(0, 1, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);

		modelMatrix = glm::mat4(1);
		if (firstPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloudsAux[i].z * 0.8f), glm::vec3(1, 0, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3((clouds3D[i].x + 0.6f) / 1.5f, clouds3D[i].y - 0.1f, clouds3D[i].x));
		}
		else if (thirdPerson) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -700, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 0.8f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 700, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(clouds3D[i].x + 0.6f, clouds3D[i].y - 0.1f, clouds3D[i].z));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(40), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z * 20), glm::vec3(1, 0, 0));
		RenderSimpleMesh2(meshes["cloud"], shaders["ShaderLight"], modelMatrix);
	}
}



bool Tema2::collisionWithPoint(float x, float y, float z, float radius)
{
	//Coordonatele centrului primei sfere care incadreaza avionul
	float ox1, oy1, oz1;
	//Coordonatele centrului celei de a doua sfere care incadreaza avionul
	float ox2, oy2, oz2;
	//Razele celor doua sfere
	float radius1, radius2;

	ox1 = planeCoordinates.x;
	oy1 = planeCoordinates.y;
	oz1 = planeCoordinates.z;
	radius1 = 1.5f;

	ox2 = planeCoordinates.x - 8 * planeScalingFactor;
	oy2 = planeCoordinates.y + 1 * planeScalingFactor;
	oz2 = planeCoordinates.z;
	radius2 = 1;

	float distance = sqrt((ox1 - x) * (ox1 - x) + (oy1 - y) * (oy1 - y) + (oz1 - z) * (oz1 - z));

	//Daca distanta dintre centrele sferelor este mai mica decat suma razelor => coliziune
	if (distance < radius1 + radius)
		return true;

	distance = sqrt((ox2 - x) * (ox2 - x) + (oy2 - y) * (oy2 - y) + (oz2 - z) * (oz2 - z));

	//Daca distanta dintre centrele sferelor este mai mica decat suma razelor => coliziune
	if (distance < radius2 + radius)
		return true;

	return false;
}



void Tema2::InitObstacle()
{
	float X, Z, N;
	X = 0.52f;
	Z = 0.85f;
	N = 0;

	float R1, G1, B1; //Darker red
	float R2, G2, B2; //Lighter red
	float R3, G3, B3; //Light red

	R1 = 133 / 255.0f; G1 = 29 / 255.0f; B1 = 24 / 255.0f;
	R2 = 184 / 255.0f; G2 = 40 / 255.0f; B2 = 29 / 255.0f;
	R3 = 219 / 255.0f; G3 = 92 / 255.0f; B3 = 83 / 255.0f;

	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-X, N, Z),  glm::vec3(0, 1, 1), glm::vec3(R1,G1,B1)),
		VertexFormat(glm::vec3(X, N,  Z),  glm::vec3(1, 0, 1), glm::vec3(R2,G2,B2)),
		VertexFormat(glm::vec3(-X, N, -Z), glm::vec3(1, 0, 0), glm::vec3(R1,G1,B1)),
		VertexFormat(glm::vec3(X, N, -Z),  glm::vec3(0, 1, 0), glm::vec3(R3,G3,B3)),
		VertexFormat(glm::vec3(N, Z, X),   glm::vec3(1, 1, 1), glm::vec3(R2,G2,B2)),
		VertexFormat(glm::vec3(N, Z, -X),  glm::vec3(0, 1, 1), glm::vec3(R1,G1,B1)),
		VertexFormat(glm::vec3(N, -Z, X),  glm::vec3(1, 1, 0), glm::vec3(R3,G3,B3)),
		VertexFormat(glm::vec3(N, -Z, -X), glm::vec3(0, 0, 1), glm::vec3(R2,G2,B2)),
		VertexFormat(glm::vec3(Z, X, N),   glm::vec3(1, 1, 1), glm::vec3(R1,G1,B1)),
		VertexFormat(glm::vec3(-Z, X, N),  glm::vec3(0, 1, 1), glm::vec3(R2,G2,B2)),
		VertexFormat(glm::vec3(Z, -X, N),  glm::vec3(1, 1, 0), glm::vec3(R2,G2,B2)),
		VertexFormat(glm::vec3(-Z, -X, N), glm::vec3(0, 0, 1), glm::vec3(R1,G1,B1))
	};

	vector<unsigned short> indices =
	{
		0,4,1,	0,9,4,	9,5,4,	4,5,8,	4,8,1,

		8,10,1,	8,3,10,	5,3,8,	5,2,3,	2,7,3,

		7,10,3,	7,6,10,	7,11,6,	11,0,6,	0,1,6,

		6,1,10,	9,0,11,	9,11,2,	9,2,5,	7,2,11
	};
	meshes["obstacle"] = Object3D::CreateMesh("obstacle", vertices, indices);
}

void Tema2::InitObstacleList()
{
	obstacleCounter = 0;
	maxObstacleY = 7;
	minObstacleY = -4;

	nrOfObstacles = 4;
	obstacles.resize(nrOfObstacles);
	obstaclesAux.resize(nrOfObstacles);
	obstaclesPos.resize(nrOfObstacles);
	obstaclesSpeed.resize(nrOfObstacles);

	/*
		Initializarea datelor care descriu obstacolele
		obstacles[i].x - coordonata X a obstacolului i inainte de rotatie
		obstacles[i].y - coordonata Y a obstacolului i inainte de rotatie
		obstacles[i].z - coordonata Z a obstacolului i inainte de rotatie


		Initializarea datelor auxiliare care descriu obstacolele
		obstaclesAux[i].x - daca a avut loc o coliziune cu respectivul obstacol
		obstaclesAux[i].y - sansa de aparitie a respectivului obstacol (pe ecran nu se va afla acelasi numar de obstacole de-a lungul jocului)
		obstaclesAux[i].z - factorul de rotatie al obstacolului
	*/
	for (int i = 0; i < nrOfObstacles; i++)
	{	
		obstaclesSpeed[i] = 1;

		obstaclesAux[i].x = false; //has collided
		obstaclesAux[i].y = rand() % 5; //cahnce to spawn
		obstaclesAux[i].z = 0; //rotation factor

		obstacles[i].x = screenOffset + rand() % 10 + (float)(rand() % 100) / 100;

		attempts = 21;
		do {
			attempts--;
			obstacles[i].y = rand() % 6 + (float)(rand() % 100) / 100;

			if (rand() % 2)
				obstacles[i].y *= -1;
		} while (!goodObstacleY(i));

		obstacles[i].z = (float)(rand() % 100) / 100;
		if (rand() % 2)
			obstacles[i].z *= -1;

		//Initializarea coordonatelor obstacolelor dupa rotatie
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(obstaclesAux[i].x), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
		modelMatrix = glm::translate(modelMatrix, obstacles[i]);
		obstaclesPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
	}
}

bool Tema2::ObstacleCollision()
{
	for (int i = 0; i < nrOfObstacles; i++)
		if (!debugMode && isObstacleVisible(i) && collisionWithPoint(obstaclesPos[i].x, obstaclesPos[i].y, obstaclesPos[i].z, 0.9f)) {
			obstaclesAux[i].x = true;
			HP--;
			return true;
		}
	return false;
}

bool Tema2::goodObstacleY(int index)
{
	if (obstacles[index].y <= 2 || attempts == 0)
		return true;

	if (obstacles[index].y < minObstacleY || obstacles[index].y > maxObstacleY)
		return false;

	if (index == 0)
		return true;

	for (int i = 0; i < index; i++)
		if (abs(obstacles[i].y - obstacles[index].y) <= 2.1f)
			return false;
	return true;
}

bool Tema2::isObstacleVisible(int index)
{
	if (obstaclesAux[index].y <= 2)
		return true;
	else
		return false;
}



void Tema2::InitFuel()
{
	InitFuelList();
	InitFuelBar();

	float R1, G1, B1;
	float R2, G2, B2;
	R1 = 37 / 255.0f;  G1 = 57 / 255.0f;  B1 = 97 / 255.0f;
	R2 = 195 / 255.0f; G2 = 206 / 255.0f; B2 = 227 / 255.0f;

	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(R1, G1, B1)),
		VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(R2, G2, B2)),
		VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(R1, G1, B1)),
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(R1, G1, B1)),
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 1,
		3, 2, 1
	};

	meshes["fuel"] = Object3D::CreateMesh("fuel", vertices, indices);
}

void Tema2::InitFuelBar()
{
	float darkR, darkG, darkB;
	float lightR, lightG, lightB;

	darkR = 20 / 155.0f;   darkG = 45 / 255.0f; darkB = 79 / 255.0f;
	lightR = 40 / 155.0f; lightG = 126 / 255.0f; lightB = 196 / 255.0f;

	//Fuel Bar
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1), glm::vec3(darkR, darkG, darkB)), //Bottom left
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(1, 0, 1), glm::vec3(lightR, lightG, lightB)), //Bottom right
			VertexFormat(glm::vec3(0,  1,  0), glm::vec3(1, 0, 0), glm::vec3(darkR, darkG, darkB)), //Top left
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 1, 0), glm::vec3(lightR, lightG, lightB)), //Top right

			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(darkR, darkG, darkB)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(lightR, lightG, lightB)),
			VertexFormat(glm::vec3(0,  1, 0), glm::vec3(1, 1, 0), glm::vec3(lightR, lightG, lightB)),
			VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 1), glm::vec3(darkR, darkG, darkB)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		meshes["fuelBar"] = Object3D::CreateMesh("fuelBar", vertices, indices);
	}

	//Fuel Bar Background
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0,  1,  0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)),

			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0,  1, 0), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		meshes["fuelBarBG"] = Object3D::CreateMesh("fuelBarBG", vertices, indices);
	}

	//Fuel Bar Outline
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1, 0,  0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(0,  1,  0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0)),

			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(0,  1, 0), glm::vec3(1, 1, 0), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		meshes["fuelBarOutline"] = Object3D::CreateMesh("fuelBarOutline", vertices, indices);
	}
}

void Tema2::InitFuelList()
{
	fuelDistance = 2.0f;
	int fuelYOffset;

	fuelYOffset = rand() % 6;

	do {
		nrOfFuel = rand() % 9;
	} while (nrOfFuel < 4);

	if (nrOfFuel >= 5)
		fuelYOffset = -1 * rand() % 3;

	fuel.resize(nrOfFuel);
	fuelAux.resize(nrOfFuel);
	fuelPos.resize(nrOfFuel);

	fuel[0].z = (float)(rand() % 100) / 100;
	if (rand() % 2)
		fuel[0].z *= -1;

	/*
		Initializarea datelor auxiliare care descriu obstacolele
		fuelAux[i].x - daca a avut loc o coliziune cu respectiva unitate de combustibil
		fuelAux[i].z - factorul de rotatie al respectivei unitati de combustibil
	*/
	for (int i = 0; i < nrOfFuel; i++)
	{
		fuel[i].z = fuel[0].z;

		fuelAux[i].x = false; //has collided
		fuelAux[i].z = 0; //rotation factor

		//Calcularea pozitiei fizice in coordonatele lumii a unitatii de combustibil in urma rotatiei
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(fuelAux[i].x), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
		modelMatrix = glm::translate(modelMatrix, fuel[i]);
		fuelPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
	}

	//faza sinusoidei
	float phase = rand() % 360;
	phase = RADIANS(phase);

	//Combustibilul va aparea ca un sir de forma sinusoidala
	fuel[0].x = 1.5f * screenOffset + rand() % 10 + (float)(rand() % 100) / 100;
	fuel[0].y = 2 * sin(fuel[0].x / 2.0f + phase) + fuelYOffset;

	/*
		Initializarea datelor care descriu unitatile de combustibil
		fuel[i].x - coordonata X a unitatii de combustibil inainte de rotatie
		fuel[i].y - coordonata Y a unitatii de combustibil inainte de rotatie
		fuel[i].z - coordonata Z a unitatii de combustibil inainte de rotatie
	*/
	for (int i = 1; i < nrOfFuel; i++)
	{
		fuel[i].x = fuel[i - 1].x + fuelDistance;
		fuel[i].y = 2 * sin(fuel[i].x / 2.0f + phase) + fuelYOffset;
	}
}

bool Tema2::isFuelVisible(int index)
{
	return !fuelAux[index].x;
}

void Tema2::FuelCollision()
{
	for (int i = 0; i < nrOfFuel; i++)
		if (isFuelVisible(i) && collisionWithPoint(fuelPos[i].x, fuelPos[i].y, fuelPos[i].z, 0.7f)) {
			fuelAux[i].x = true;
			//factorul de umplere al barei de combustibil
			fuelBarScalingFactor += 15.0f;

			//nu se poate depasi mai mult decat capacitatea maxima a rezervorului
			if (fuelBarScalingFactor >= 100.0f)
				fuelBarScalingFactor = 100.0f;
		}
}



void Tema2::drawHP()
{
	if (HP >= 1) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-31, 16, -10));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(heartRotationFactor), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90), glm::vec3(1, 0, 0));
		RenderSimpleMeshColor(meshes["heart"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0, 0));
	}

	if (HP >= 2) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-27, 16, -10));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(heartRotationFactor), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90), glm::vec3(1, 0, 0));
		RenderSimpleMeshColor(meshes["heart"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0, 0));
	}

	if (HP == 3) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-23, 16, -10));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(heartRotationFactor), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90), glm::vec3(1, 0, 0));
		RenderSimpleMeshColor(meshes["heart"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0, 0));
	}
}




float Tema2::getMouseWorldPos()
{
	glm::mat4 Projection = glm::perspective(1.0f, 4.0f / 3.0f, 1.0f, 1024.1f);
	glm::mat4 GlobalProjection = Projection;

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 22), // Camera location
		glm::vec3(0, 1, 0), // Where camera is looking
		glm::vec3(0, 1, 0) // Camera head is facing up
	);
	glm::mat4 GlobalView = View;

	// NORMALISED DEVICE SPACE
	double x = 2.0 * (cursorX) / resolution.x - 1;
	double y = 2.0 * (cursorY) / resolution.y - 1;


	// HOMOGENEOUS SPACE
	glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

	// Projection/Eye Space
	glm::mat4 ProjectView = GlobalProjection * GlobalView;
	glm::mat4 viewProjectionInverse = inverse(ProjectView);

	glm::vec4 worldPos = viewProjectionInverse * screenPos;

	return (worldPos.y * 10.0f * (resolution.x / (resolution.y - 180.0f)));
}

void Tema2::Init()
{	
	resolution = window->GetResolution();
	attempts = 21;
	screenOffset = 23;

	InitPlane();
	InitObstacle();
	InitObstacleList();
	InitFuel();
	InitSky();
	InitClouds();

	polygonMode = GL_FILL;
	wait = 0;
	waitTime = 10;
	gameOver = false;
	initialAnimation = true;
	firstPerson = false;
	thirdPerson = true;
	switchCamera = false;
	rotatePlane = true;
	currentOption = 0;
	debugMode = false;
	HP = 3;
	meshes["planeHitBox"] = Object3D::CreateSphere("planeHitBox", 1, 100, 100);
	
	waterRotationFactor = 0.0f;
	meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 100, 500, true);
	
	thirdPersonCamera = new Laborator::MyCamera();
	thirdPersonCamera->Set(glm::vec3(0, 0, 22), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	firstPersonCamera = new Laborator::MyCamera();
	firstPersonCamera->Set(glm::vec3(planeX + 0.1f, planeY + 1.2f, planeCoordinates.z), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderLight");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderLab7");
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("heart");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props", "heart2.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 17, -2);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	resolution = window->GetResolution();
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	if (!gameOver)
	{	
		//Cat timp nu este GameOver se deseneaza avionul
		//drawPlaneHitBox();
		drawPlanePart("planeCrossBG");
		drawPlanePart("planeBody");
		drawPlanePart("planeFront");
		drawPlanePart("planeTailSupport");
		drawPlanePart("planeTail");
		drawPlanePart("planeWing");
		drawPlanePart("planeWingSupport");
		drawPlanePart("Wheels");
		
		if(planeRotationFactor >= 5)
			propellerRotationFactor -= 20 * deltaTimeSeconds; //avionul urca => elice se invarte mai tare
		else if(planeRotationFactor < -5) 
			propellerRotationFactor -= 5 * deltaTimeSeconds;  //avionul coboara => elicea se invarte mai incet
		else
			propellerRotationFactor -= 10 * deltaTimeSeconds; //avionul isi mentine inaltimea

		drawPropeller();

		//Animatia initiala
		if (initialAnimation)
		{	
			//Avionul incepe din stanga
			planeCoordinates.x += 15.0f * deltaTimeSeconds;

			//Avionul nu trebuie sa depaseasca pozitia stabila
			if (planeCoordinates.x >= planeX)
				planeCoordinates.x = planeX;

			//planeRotationFactor = where_the_mouse_is_suposed_to_be - planeCoorindates.y;
			planeRotationFactor = 2 - planeCoordinates.y;
			planeRotationFactor *= 6.0f;

			//Ajustarea coordonate Y a avionului
			if (abs(planeRotationFactor) > 0 && planeCoordinates.y < planeY)
				planeCoordinates.y += 15.0f * deltaTimeSeconds;

			//Avionul nu trebuie sa depaseasca coordonata Y tinta
			if (planeCoordinates.y >= planeY)
				planeCoordinates.y = planeY;

			//Odata ajuns la pozitia dorita se asteapta putin
			if (planeCoordinates.x == planeX && planeCoordinates.y == planeY)
				wait++;

			//A trecut suficient timp de la ajungerea avionului in pozitia dorita (planeX, planeY)
			if (wait >= waitTime)
				initialAnimation = false;
		}
		else
		{	
			//Pe masura ce trece timpul => bara de combustibil continua sa se goleasca
			fuelBarScalingFactor -= 3.0f * deltaTimeSeconds;
			
			//In cazul unei coliziuni, avionul este impis catre stanga fata de pozitia curent
			if (ObstacleCollision()) {
				newPlaneX = planeCoordinates.x - 4;
				hit = true;
				comingBack = false;
			}

			//Animatia de deplasare catre stanga
			if (hit == true && planeCoordinates.x > newPlaneX) {
				planeCoordinates.x -= 30 * deltaTimeSeconds;
				firstPersonCamera->TranslateForward(-30.0f * deltaTimeSeconds);

				if (planeCoordinates.x <= newPlaneX)
					comingBack = true;
			}

			//Intoarcerea catre pozitia originala
			if (comingBack) {	
				planeCoordinates.x += 15 * deltaTimeSeconds;
				firstPersonCamera->TranslateForward(15.0f * deltaTimeSeconds);
				hit = false;
			}

			//Avionul nu trebuie sa depaseasca pozitia initiala
			if (planeCoordinates.x >= planeX) {
				planeCoordinates.x = planeX;
				comingBack = false;
			}
			
			//Coliziunea cu unitatile de combustibil
			FuelCollision();

			if (fuelBarScalingFactor <= 0.0f || HP == 0) {	
				gameOver = true;
				finalAnimation = true;
			}
			
			//Factorul de rotatie se calculeaza ca diferenta dintre pozitia cursorului in coordonatele scenei si pozitia curenta a avionului
			mouse = getMouseWorldPos();
			planeRotationFactor = mouse - planeCoordinates.y;
			planeRotationFactor *= 6.0f;

			firstPersonCamera->TranslateUpword(planeRotationFactor * 1.8f * deltaTimeSeconds);

			//Deplasarea catre cursor se face in functie de gradul de rotatie
			if (abs(planeRotationFactor) > 0)
				planeCoordinates.y += planeRotationFactor * 1.8f * deltaTimeSeconds;


			//Draw the obstacles
			for (int i = 0; i < nrOfObstacles; i++)
			{	
				//Initializarea unui nou obstacol la pasariea spatiului vizual
				if (obstaclesPos[i].x < -screenOffset || obstaclesAux[i].x)
				{
					if (i == nrOfObstacles - 1)
						obstacleCounter++;

					if (obstacleCounter == 3) {
						obstacleCounter = 0;
						fuelSpeed += 0.05f;
						InitFuel();
					}

					if(obstaclesSpeed[i] < 3)
						obstaclesSpeed[i] += 0.05f;

					obstaclesAux[i].x = false; //has collided
					obstaclesAux[i].y = rand() % 5; //cahnce to spawn
					obstaclesAux[i].z = 0; //rotation factor

					obstacles[i].x = screenOffset + rand() % 10 + (float)(rand() % 100) / 100;

					attempts = 21;
					do {
						attempts--;
						obstacles[i].y = rand() % 6 + (float)(rand() % 100) / 100;

						if (rand() % 2)
							obstacles[i].y *= -1;
					} while (!goodObstacleY(i));

					obstacles[i].z = (float)(rand() % 100) / 100;
					if (rand() % 2)
						obstacles[i].z *= -1;

					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(obstaclesAux[i].z), glm::vec3(0, 0, 1));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
					modelMatrix = glm::translate(modelMatrix, obstacles[i]);
					obstaclesPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);

				}
				else if (!obstaclesAux[i].x)
				{	
					//Daca un obstacol este vizibl, acesta este desenat
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(obstaclesAux[i].z * obstaclesSpeed[i]), glm::vec3(0, 0, 1));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
					modelMatrix = glm::translate(modelMatrix, obstacles[i]);
					modelMatrix = glm::rotate(modelMatrix, RADIANS(obstaclesAux[i].z * 100), glm::vec3(1, 0, 0));

					obstaclesPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);

					if (isObstacleVisible(i))
						RenderSimpleMesh2(meshes["obstacle"], shaders["ShaderLight"], modelMatrix);
				}
			}

			for (int i = 0; i < nrOfObstacles; i++)
				obstaclesAux[i].z += 5 * deltaTimeSeconds;

			//Draw the Fuel
			for (int i = 0; i < nrOfFuel; i++)
			{	
				//La parasirea spatiului vizual al ultimei unitati de combustibil din sir se astepata o noua initializare
				if (i == nrOfFuel - 1 && fuelPos[i].x < -screenOffset - 3)
				{
					nrOfFuel = 0;
				}
				else
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -300, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(fuelAux[i].z * fuelSpeed), glm::vec3(0, 0, 1));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 300, 0));
					modelMatrix = glm::translate(modelMatrix, fuel[i]);
					modelMatrix = glm::rotate(modelMatrix, RADIANS(fuelAux[i].z * 3.5f), glm::vec3(1, 1, 0));

					fuelPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
					if (isFuelVisible(i))
						RenderSimpleMesh2(meshes["fuel"], shaders["ShaderLight"], modelMatrix);
				}
			}

			for (int i = 0; i < nrOfFuel; i++)
				fuelAux[i].z += 3 * deltaTimeSeconds;
		}
	}
	else
	{	
		//Final animation
		if (finalAnimation)
		{	
			propellerRotationFactor -= 2 * deltaTimeSeconds;
			planeCoordinates.x += 5 * deltaTimeSeconds;
			firstPersonCamera->RotateThirdPerson_OX(-0.2 * deltaTimeSeconds);
			fallingRotationFactor -= 200 * deltaTimeSeconds;

			drawFallingPlanePart("planeCrossBG", fallingRotationFactor);
			drawFallingPlanePart("planeBody", fallingRotationFactor);
			drawFallingPlanePart("planeFront", fallingRotationFactor);
			drawFallingPlanePart("planeTailSupport", fallingRotationFactor);
			drawFallingPlanePart("planeTail", fallingRotationFactor);
			drawFallingPlanePart("planeWing", fallingRotationFactor);
			drawFallingPlanePart("Wheels", fallingRotationFactor);
			drawPropeller();
			
			planeRotationFactor = -30;

			if (abs(planeRotationFactor) > 0) {
				planeCoordinates.y += planeRotationFactor * 0.2f * deltaTimeSeconds;
			}

			if (planeCoordinates.y <= -20)
				finalAnimation = false;
		}
	}
	
	//Draw Clouds
	for (int i = 0; i < nrOfClouds; i++)
	{
		if ((firstPerson && cloudsPos[i].z < -3.5f * screenOffset) || (thirdPerson && cloudsPos[i].x < -1.5f * screenOffset))
		{
			//Init new Cloud
			cloudsAux[i].x = rand() % 5; //chance to spawn and cloud type

			clouds3D[i].x = 1.5f * screenOffset + (rand() % 4 + 1) * (rand() % 40 + (float)(rand() % 100) / 100);

			if (clouds3D[i].x - 1.5f * screenOffset >= 80)
				clouds3D[i].x /= 2.0f;

			attempts = 21;
			do {
				attempts--;
				clouds3D[i].y = rand() % 10 + (float)(rand() % 100) / 100;
			} while (clouds3D[i].y >= 6 || attempts > 0);

			do {
				clouds3D[i].z = -2 * rand() % 10 - (float)(rand() % 100) / 100;
			} while (clouds3D[i].z >= -3);


			cloudsAux[i].z = 0;

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -100, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(cloudsAux[i].z), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 100, 0));
			modelMatrix = glm::translate(modelMatrix, clouds3D[i]);
			cloudsPos[i] = modelMatrix * glm::vec4(0, 0, 0, 1);
		} 
		else
		{	
			drawCloud(i);
		}
	}

	for (int i = 0; i < nrOfClouds; i++)
		cloudsAux[i].z += 1.2f * deltaTimeSeconds;

	//Desenarea apei peste care zboara avionul
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		if (firstPerson)
			modelMatrix *= Transform3D::RotateOY(RADIANS(-90));
		modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -400));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(120));
		modelMatrix *= Transform3D::RotateOY(RADIANS(waterRotationFactor));
		RenderSimpleMesh2(meshes["water"], shaders["ShaderTema2"], modelMatrix);
	}

	//Desenarea barei de combustibil
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 4, 14));
		modelMatrix *= Transform3D::ScaleX(0.05f * fuelBarScalingFactor);  
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		modelMatrix *= Transform3D::RotateOY(RADIANS(180));
		RenderSimpleMesh(meshes["fuelBar"], shaders["VertexNormal"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 4, 14));
		modelMatrix *= Transform3D::ScaleX(0.05f * 100);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		modelMatrix *= Transform3D::RotateOY(RADIANS(180));
		RenderSimpleMesh(meshes["fuelBarBG"], shaders["VertexNormal"], modelMatrix);
	}

	/*
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8 + 0.04f, 4 - 0.025f, 14 - 0.008f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.02f)); 
		modelMatrix *= Transform3D::ScaleX(0.046f * 100);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.55f));
		modelMatrix *= Transform3D::RotateOY(RADIANS(180));
		RenderSimpleMesh(meshes["fuelBarOutline"], shaders["VertexNormal"], modelMatrix);
	}
	*/

	//Desenarea cubului care descrie cerul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderSimpleMesh2(meshes["sky"], shaders["ShaderLight"], modelMatrix);
	}

	//Rotation Of the water
	waterRotationFactor -= 5 * deltaTimeSeconds;

	//Odata la implinirea unei rotatii complete se reinitializeaza marea, avand o noua structura generala fata de cea inlocuita
	if (-waterRotationFactor > 360)
	{	
		int option;
		do {
			option = rand() % 4;
		} while (option == currentOption); //Pentru a nu avea acelsi tip de mare de doua ori la rand

		if(option == 0)
			meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 100, 500, true);
		else if(option == 1)
			meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 100, 70, true);
		else if(option == 2)
			meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 60, 90, true);
		else if(option == 4)
			meshes["water"] = Object3D::CreateCylinder("water", 3, 5, 35, 55, true);
		waterRotationFactor = 0;
	}

	// Render the point light in the scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
		RenderSimpleMesh2(meshes["sphere"], shaders["ShaderLight"], modelMatrix);

	}

	//Draw the HP
	{
		heartRotationFactor -= 80 * deltaTimeSeconds;
		drawHP();
	}
	
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int locationModel = glGetUniformLocation(shader->program, "Model");
	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	int locationView = glGetUniformLocation(shader->program, "View");
	// TODO : set shader uniform "View" to viewMatrix

	glm::mat4 viewMatrix;

	if (switchCamera) {
		viewMatrix = GetSceneCamera()->GetViewMatrix();
	}
	else {
		viewMatrix = thirdPersonCamera->GetViewMatrix();
	}

	glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	int locationProjection = glGetUniformLocation(shader->program, "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float time = Engine::GetElapsedTime();
	int locationTime = glGetUniformLocation(shader->program, "Time");
	glUniform1f(locationTime, time);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderSimpleMesh2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	if (strcmp(mesh->GetMeshID(), "planeBody") == 0)
	{
		materialShininess = 10;
		materialKd = 0.5;
		materialKs = 0.07f;
	}
	else if (strcmp(mesh->GetMeshID(), "sky") == 0)
	{
		materialShininess = 60;
		materialKd = 0.2f;
		materialKs = 0.2f;
	}
	else if (strcmp(mesh->GetMeshID(), "water") == 0)
	{
		materialShininess = 1;
		materialKd = 0.5f;
		materialKs = 0.75f;
	}
	else if (strcmp(mesh->GetMeshID(), "fuel") == 0)
	{
		materialShininess = 10;
		materialKd = 0.8f;
		materialKs = 0.85f;
	} 
	else  if (strcmp(mesh->GetMeshID(), "cloud") == 0)
	{
		materialShininess = 30;
		materialKd = 0.9f;
		materialKs = 0.85f;
	}


	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition;
	if(thirdPerson)
		eyePosition = thirdPersonCamera->GetTargetPosition();
	else if(firstPerson)
		eyePosition = firstPersonCamera->GetTargetPosition();

	GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	GLint loc = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(loc, materialShininess);

	loc = glGetUniformLocation(shader->program, "material_kd");  // componenta difuza
	glUniform1f(loc, materialKd);

	loc = glGetUniformLocation(shader->program, "material_ks");  // componenta speculara
	glUniform1f(loc, materialKs);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glm::mat4 viewMatrix;

	if (switchCamera) {
		viewMatrix = GetSceneCamera()->GetViewMatrix();
	}
	else {
		viewMatrix = thirdPersonCamera->GetViewMatrix();
		if (thirdPerson)
			viewMatrix = thirdPersonCamera->GetViewMatrix();
		else if (firstPerson)
			viewMatrix = firstPersonCamera->GetViewMatrix();
	}

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float time = Engine::GetElapsedTime();
	int locationTime = glGetUniformLocation(shader->program, "Time");
	glUniform1f(locationTime, time);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderSimpleMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	if (strcmp(mesh->GetMeshID(), "heart") == 0)
	{
		materialShininess = 10;
		materialKd = 0.5;
		materialKs = 0.9f;
	}
	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	// TODO: Set eye position (camera position) uniform
	//glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	glm::vec3 eyePosition = thirdPersonCamera->GetTargetPosition();

	GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	GLint loc = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(loc, materialShininess);

	loc = glGetUniformLocation(shader->program, "material_kd");  // componenta difuza
	glUniform1f(loc, materialKd);

	loc = glGetUniformLocation(shader->program, "material_ks");  // componenta speculara
	glUniform1f(loc, materialKs);

	loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glm::mat4 viewMatrix;

	if (switchCamera) {
		viewMatrix = GetSceneCamera()->GetViewMatrix();
	}
	else {
		viewMatrix = thirdPersonCamera->GetViewMatrix();
	}

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float time = Engine::GetElapsedTime();
	int locationTime = glGetUniformLocation(shader->program, "Time");
	glUniform1f(locationTime, time);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::FrameEnd()
{	
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_1)) meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 100, 500, true);
	if (window->KeyHold(GLFW_KEY_2)) meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 100, 70, true);
	if (window->KeyHold(GLFW_KEY_3)) meshes["water"] = Object3D::CreateCylinder("water", 2.9f, 5, 60, 90, true);
	if (window->KeyHold(GLFW_KEY_4)) meshes["water"] = Object3D::CreateCylinder("water", 3, 5, 35, 55, true);
	if (window->KeyHold(GLFW_KEY_5)) {
		fallingWings = !fallingWings;
		//cout << fallingWings << endl;
	}

	if (window->KeyHold(GLFW_KEY_K)) {
		switchCamera = !switchCamera;
	}

	if (window->KeyHold(GLFW_KEY_C)) {
		if (!initialAnimation) {
			firstPerson = !firstPerson;
			thirdPerson = !thirdPerson;
		}
	}

	if (window->KeyHold(GLFW_KEY_R)) {
		rotatePlane = !rotatePlane;
	}

	if (window->KeyHold(GLFW_KEY_P)) {
		debugMode = !debugMode;
	}

	if (key == GLFW_KEY_SPACE){
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

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	cursorX = mouseX;
	cursorY = mouseY;

	if (cursorY < (resolution.y / 7.0f))
		cursorY = (resolution.y / 7.0f);

	if (cursorY > (resolution.y) - (resolution.y / 4.0f))
		cursorY = (resolution.y) - (resolution.y / 4.0f);
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
