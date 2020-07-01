#include "Tema3.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>
#include <time.h>
#include <string>

#include "../Laborator4/Transform3D.h"

using namespace std;

//Path-ul catre texturi
const string textureLoc = "Source/Laboratoare/Tema3/Textures/";

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

//Incarcarea imaginii pentru deformare
void Tema3::LoadHeightMap()
{
	heightPixels = new unsigned char[width * height];
	heightPixels = stbi_load((textureLoc + "heightmap.png").c_str(), &width, &height, &channels, STBI_grey);
	
	Texture2D* texture = new Texture2D();
	texture->Load2DFromData(heightPixels, width, height, channels, GL_CLAMP_TO_EDGE);
	mapTextures["heightmap"] = texture;
}

//Reincarcarea Texturilor
void Tema3::ReloadHeightMap()
{	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, (void*)heightPixels);
	
	Texture2D* texture = new Texture2D();
	texture->Load2DFromData(heightPixels, width, height, channels, GL_CLAMP_TO_EDGE);
	mapTextures["heightmap"] = texture;
	
	texture = new Texture2D();
	texture->Load2D((textureLoc + "earth_texture3.jpg").c_str(), GL_REPEAT);
	mapTextures["earth"] = texture;
	
	texture = new Texture2D();
	texture->Load2D((textureLoc + "sky2.jpg").c_str(), GL_REPEAT);
	mapTextures["sky"] = texture;
}

//Obtinerea indexului in vectorul de heightMap pentru niste coordonate in spatiul lumii
int Tema3::GetIndex(float x, float z)
{
	int newX = ((x / maxPlaneX) + 1.0f) * 128.0f;
	int newZ = ((z / maxPlaneZ) + 1.0f) * 128.0f;

	return newZ * 256 + newX;
}

//Setarea camerei third Person
void Tema3::SetThirdPersonCamera()
{
	if (!projectileAnimation) {
		if (turn == 2) { //Player 1 turn
			camera1->Set(glm::vec3(3, 4.2, -2.7), player1Position, glm::vec3(0, 1, 0));
			player1RotationFactor.x = player1RotationFactor.y = 0;
			turn = 1;
		}
		else if (turn == 1) { //Player 2 turn
			camera2->Set(glm::vec3(1.1401, 5.1, -16.8037), player2Position, glm::vec3(0, 1, 0));
			player2RotationFactor.x = player2RotationFactor.y = 0;
			turn = 2; 
		}
	}
}

void Tema3::Init()
{	
	polygonMode = GL_FILL;
	srand(time(NULL));
	LoadHeightMap();

	//Coordonatele caracterelor (player 1 - Verde)
	//							(player 2 - Portocaliu)
	player1Position = glm::vec3(2.3f, 3.95f, -3.5f);
	player2Position = glm::vec3(0.73f, 4.45f, -16.0f);
	//Initial este tura player-ului 1
	projectilePosition = player1Position;

	//Player 1 Camera
	camera1 = new Laborator::MyCameraTema3();
	camera1->Set(glm::vec3(3, 4.2, -2.7), glm::vec3(0,0,0), glm::vec3(0, 1, 0));
	camera1->RotateFirstPerson_OX(0.595);
	camera1->RotateFirstPerson_OY(-1.549);

	//Player 2 Camera
	camera2 = new Laborator::MyCameraTema3();
	camera2->Set(glm::vec3(1.1401, 5.1, -16.8037), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	fallOff = 0;
	turn = 1;

	//Player model
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Earth Texture
	{
		Texture2D* texture1 = new Texture2D();
		texture1->Load2D((textureLoc + "earth_texture3.jpg").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture1;
	}

	//Sky Texture
	{
		Texture2D* texture2 = new Texture2D();
		texture2->Load2D((textureLoc + "sky2.jpg").c_str(), GL_REPEAT);
		mapTextures["sky"] = texture2;
	}

	//Projectile
	{
		Mesh* mesh = new Mesh("projectile");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//The light source
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Spotlight
	{
		Mesh* mesh = new Mesh("spotlight");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Plane (Ground)
	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;

		//Edge plane coordinates
		minPlaneZ = maxPlaneZ = (float)((mesh->positions)[0]).z;
		minPlaneX = maxPlaneX = (float)((mesh->positions)[0]).x;
		
		for (int i = 0; i < mesh->positions.size(); i++) {
			if (maxPlaneX < (float)((mesh->positions)[i]).x)
				maxPlaneX = (float)((mesh->positions)[i]).x;

			if (maxPlaneZ < (float)((mesh->positions)[i]).z)
				maxPlaneZ = (float)((mesh->positions)[i]).z;

			if (minPlaneX > (float)((mesh->positions)[i]).x)
				minPlaneX = (float)((mesh->positions)[i]).x;

			if (minPlaneZ < (float)((mesh->positions)[i]).z)
				minPlaneZ = (float)((mesh->positions)[i]).z;
		}
		
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema3Terrain");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShaderTerrain.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShaderTerrain.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema3Sky");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShaderSky.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShaderSky.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 20, 1);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;

		spotlight1Position = glm::vec3(3.4, 5.3, -3.1);
		spotlight1Direction = glm::vec3(-0.57, -0.79, -0.21);

		spotlight2Position = glm::vec3(1.6, 4.9, -15);
		spotlight2Direction = glm::vec3(-0.6, -0.43, -0.67);
	}
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{	
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	//Rotatia cerului
	t += 0.008;
	if (t >= 1000)
		t = 0;

	//Player1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, player1Position);

		//Cat timp este randul sau iar proiectilul nu se deplaseaza, caracterul se poate roti
		if (turn == 1 && !projectileAnimation) {
			modelMatrix *= Transform3D::RotateOX(RADIANS(player1RotationFactor.x));
			modelMatrix *= Transform3D::RotateOY(RADIANS(player1RotationFactor.y));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, glm::vec3(0.123, 0.323, 0.18));
	}

	//Player2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, player2Position);

		//Cat timp este randul sau iar proiectilul nu se deplaseaza, caracterul se poate roti
		if (turn == 2 && !projectileAnimation) {
			modelMatrix *= Transform3D::RotateOX(RADIANS(player2RotationFactor.x));
			modelMatrix *= Transform3D::RotateOY(RADIANS(player2RotationFactor.y));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, glm::vec3(0.833, 0.533, 0.128));
	}

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		RenderSimpleMeshWithTexture(meshes["plane"], shaders["ShaderTema3Terrain"], modelMatrix, glm::vec3(0.213, 0.45, 0.723), mapTextures["heightmap"], mapTextures["earth"]);
	} 

	//RenderSky
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(370.0f));
		RenderSky(meshes["sphere"], shaders["ShaderTema3Sky"], modelMatrix, mapTextures["sky"]);
	}

	// Render the point light in the scene
	/*
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, glm::vec3(0.17, 0.43, 0.83));
	}
	*/

	//Clicked - proiectil lansat
	if (projectileAnimation) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, projectilePosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["projectile"], shaders["ShaderTema3"], modelMatrix, glm::vec3(1));

		projectilePosition +=  projectileDirection * 0.2f;
		projectilePosition.y -= fallOff;
		fallOff += 0.001f;

		if(turn == 1)
			camera1->Set((projectilePosition + 2.0f), projectilePosition, glm::vec3(0, 1, 0));
		else
			camera2->Set((projectilePosition + 2.0f), projectilePosition, glm::vec3(0, 1, 0));
	}

	//Collision check
	if (projectileAnimation && heightPixels[GetIndex(projectilePosition.x, projectilePosition.z)] / 255.0f * 6.5f > projectilePosition.y) {
		//Se deformeaza o zona in jurul punctului unde s-a produs coliziunea
		for(float i = projectilePosition.x - 0.3f; i < projectilePosition.x + 0.3f; i = i + 0.2f)
			for (float j = projectilePosition.z - 0.3f; j < projectilePosition.z + 0.3f; j = j + 0.2f)
				if (GetIndex(i, j) > 0 && GetIndex(i, j) < width * height) { //Se verifica faptul ca index-ul se afla in interiorul memoriei alocate
					if ((int)heightPixels[GetIndex(i, j)] >= 30) {
						heightPixels[GetIndex(i, j)] -= 30;
					}
					else {
						heightPixels[GetIndex(i, j)] = 0;
					}
				}
	
		ReloadHeightMap();
		projectileAnimation = false;
		startTimer = true; //Se porneste timer-ul pentru vizualizarea deformarii
	}

	if (startTimer) {
		timer += 0.1;

		if (timer > 3) {
			SetThirdPersonCamera();
			timer = 0;
			startTimer = false;
		}
		if (turn == 1)
			projectilePosition = player1Position;
		else
			projectilePosition = player2Position;

		//Gradul de cadere al proiectilului se reseteaza
		fallOff = 0;
	}
}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int lighting_type_position = glGetUniformLocation(shader->program, "lighting_type");
	glUniform1i(lighting_type_position, lightingType);

	int cutoff_position = glGetUniformLocation(shader->program, "cut_off");
	glUniform1f(cutoff_position, cutoff);

	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	//Spot light 1
	int spotlight1_position = glGetUniformLocation(shader->program, "spotlight1_position");
	glUniform3f(spotlight1_position, spotlight1Position.x, spotlight1Position.y, spotlight1Position.z);

	int spotlight1_direction = glGetUniformLocation(shader->program, "spotlight1_direction");
	glUniform3f(spotlight1_direction, spotlight1Direction.x, spotlight1Direction.y, spotlight1Direction.z);

	//Spot light 2
	int spotlight2_position = glGetUniformLocation(shader->program, "spotlight2_position");
	glUniform3f(spotlight2_position, spotlight2Position.x, spotlight2Position.y, spotlight2Position.z);

	int spotlight2_direction = glGetUniformLocation(shader->program, "spotlight2_direction");
	glUniform3f(spotlight2_direction, spotlight2Direction.x, spotlight2Direction.y, spotlight2Direction.z);
	
	// TODO: Set eye position (camera1 position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix;
	
	if (turn == 1) {
		viewMatrix = camera1->GetViewMatrix();
	}
	else if (turn == 2) {
		viewMatrix = camera2->GetViewMatrix();
	}
	else
		viewMatrix = GetSceneCamera()->GetViewMatrix();

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMeshWithTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture1, Texture2D* texture2)
{	
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int lighting_type_position = glGetUniformLocation(shader->program, "lighting_type");
	glUniform1i(lighting_type_position, lightingType);

	int cutoff_position = glGetUniformLocation(shader->program, "cut_off");
	glUniform1f(cutoff_position, cutoff);

	
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);
	
	
	//Spot light 1
	int spotlight1_position = glGetUniformLocation(shader->program, "spotlight1_position");
	glUniform3f(spotlight1_position, spotlight1Position.x, spotlight1Position.y, spotlight1Position.z);

	int spotlight1_direction = glGetUniformLocation(shader->program, "spotlight1_direction");
	glUniform3f(spotlight1_direction, spotlight1Direction.x, spotlight1Direction.y, spotlight1Direction.z);

	//Spot light 2
	int spotlight2_position = glGetUniformLocation(shader->program, "spotlight2_position");
	glUniform3f(spotlight2_position, spotlight2Position.x, spotlight2Position.y, spotlight2Position.z);

	int spotlight2_direction = glGetUniformLocation(shader->program, "spotlight2_direction");
	glUniform3f(spotlight2_direction, spotlight2Direction.x, spotlight2Direction.y, spotlight2Direction.z);
	

	// TODO: Set eye position (camera1 position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();;

	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix;
	
	if (turn == 1) {
		viewMatrix = camera1->GetViewMatrix();
	}
	else if (turn == 2) {
		viewMatrix = camera2->GetViewMatrix();
	}
	else
		viewMatrix = GetSceneCamera()->GetViewMatrix();

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1) {	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	}

	if (texture2) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSky(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix;

	if (turn == 1)
		viewMatrix = camera1->GetViewMatrix();
	else if (turn == 2)
		viewMatrix = camera2->GetViewMatrix();
	else
		viewMatrix = GetSceneCamera()->GetViewMatrix();

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int loc_time = glGetUniformLocation(shader->program, "time");
	glUniform1f(loc_time, t);

	int loc_flag = glGetUniformLocation(shader->program, "v_flag");
	glUniform1i(loc_flag, spin);

	if (texture1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	}

	if (texture2) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{	
	float camera1Speed = 2.0f;
	//Deplasarea camerelor adaugate
	// move the camera1 only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float distance = camera1Speed * deltaTime;

		if (window->KeyHold(GLFW_KEY_W)) {
			if(turn == 1) 
				camera1->MoveForward(distance);
			else
				camera2->MoveForward(distance);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			if (turn == 1)
				camera1->TranslateRight(-distance);
			else
				camera2->TranslateRight(-distance);	
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			if (turn == 1)
				camera1->MoveForward(-distance);
			else
				camera2->MoveForward(-distance);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			if (turn == 1)
				camera1->TranslateRight(distance);
			else
				camera2->TranslateRight(distance);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			if (turn == 1)
				camera1->TranslateUpword(-distance);
			else
				camera2->TranslateUpword(-distance);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			if (turn == 1)
				camera1->TranslateUpword(distance);
			else
				camera2->TranslateUpword(distance);
		}
	}

	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) spotlight1Position -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) spotlight1Position -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) spotlight1Position += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) spotlight1Position += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) spotlight1Position += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) spotlight1Position -= up * deltaTime * speed;


		glm::mat4 rotationMatrix(1);
		float speed = 2.5f;

		if (window->KeyHold(GLFW_KEY_Y)) rotationMatrix = glm::rotate(glm::mat4(1), deltaTime * speed, glm::vec3(1, 0, 0));
		if (window->KeyHold(GLFW_KEY_H)) rotationMatrix = glm::rotate(glm::mat4(1), -deltaTime * speed, glm::vec3(1, 0, 0));
		if (window->KeyHold(GLFW_KEY_G)) rotationMatrix = glm::rotate(glm::mat4(1), deltaTime * speed, glm::vec3(0, 1, 0));
		if (window->KeyHold(GLFW_KEY_J)) rotationMatrix = glm::rotate(glm::mat4(1), -deltaTime * speed, glm::vec3(0, 1, 0));
		spotlight1Direction = glm::normalize(rotationMatrix * glm::vec4(spotlight1Direction, 0));

		if (window->KeyHold(GLFW_KEY_Z)) cutoff -= deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_X)) cutoff += deltaTime * speed;
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		SetThirdPersonCamera();
	}

	if (key == GLFW_KEY_P) {
		//Debug Information 
		cout << "camera1: ";
		cout << camera1->position.x << " " << camera1->position.y << " " << camera1->position.z << endl << endl;

		cout << "camera2: ";
		cout << camera2->position.x << " " << camera2->position.y << " " << camera2->position.z << endl << endl;

		cout << "PLAYER 1: " << endl;
		cout << "Player 1 Position: " << player1Position.x << " " << player1Position.y << " " << player1Position.z << endl << endl;
		cout << "Spotlight: ";
		cout << "Position: " << spotlight1Position.x << " " << spotlight1Position.y << " " << spotlight1Position.z << endl;
		cout << "Direction: " << spotlight1Direction.x << " " << spotlight1Direction.y << " " << spotlight1Direction.z << endl << endl;
		
		cout << "Cut off: " << cutoff << endl;

		cout << "PLAYER 2: " << endl;
		cout << "Player 2 Position: " << player2Position.x << " " << player2Position.y << " " << player2Position.z << endl << endl;
		cout << "Spotlight: ";
		cout << "Position: " << spotlight2Position.x << " " << spotlight2Position.y << " " << spotlight2Position.z << endl;
		cout << "Direction: " << spotlight2Direction.x << " " << spotlight2Direction.y << " " << spotlight2Direction.z << endl;
	}

	//Swap cameras
	//turn 3 - Debug
	if (key == GLFW_KEY_C) {
		if (turn == 1 || turn == 2)
			turn = 3;
		else
			turn = 1;
	}

	//Lower entire map by 10
	if (key == GLFW_KEY_K) {
		//cout << "RELOAD" << endl;
		int h = 1;

		int size = width * height;
		for (int i = 0; i < size; i++) {
		
			if ((int)heightPixels[i] > 10)
				heightPixels[i] -= 10;
		}
		ReloadHeightMap();
	}

	//Raise entire map by 10
	if (key == GLFW_KEY_L) {
		//cout << "RELOAD" << endl;
		int h = 1;

		int size = width * height;
		for (int i = 0; i < size; i++) {

			if ((int)heightPixels[i] < 240)
				heightPixels[i] += 10;
		}
		ReloadHeightMap();
	}

	if (key == GLFW_KEY_SPACE) {
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

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL && !projectileAnimation) {
			if (turn == 1) {
				camera1->RotateFirstPerson_OX(-sensivityOX * deltaY);
				camera1->RotateFirstPerson_OY(-sensivityOY * deltaX);
			}
			
			if (turn == 2) {
				camera2->RotateFirstPerson_OX(-sensivityOX * deltaY);
				camera2->RotateFirstPerson_OY(-sensivityOY * deltaX);
			}

			//rotationX += -sensivityOX * deltaY;
			//rotationY += -sensivityOY * deltaX;
			//cout << rotationX << " " << rotationY << endl;
		}

		if (window->GetSpecialKeyState() == 0 && !projectileAnimation) {
			if (turn == 1) {
				camera1->RotateThirdPerson_OX(-sensivityOX * 0.3f * deltaY);
				camera1->RotateThirdPerson_OY(-sensivityOY * 0.3f * deltaX);
				
				player1RotationFactor.x -= sensivityOX * 25 * deltaY;
				player1RotationFactor.y -= sensivityOY * 25 * deltaX;
			}

			if (turn == 2) {
				camera2->RotateThirdPerson_OX(-sensivityOX * 0.3f * deltaY);
				camera2->RotateThirdPerson_OY(-sensivityOY * 0.3f * deltaX);

				player2RotationFactor.x += sensivityOX * 25 * deltaY;
				player2RotationFactor.y -= sensivityOY * 25 * deltaX;
			}	
		}

	}
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && (turn == 1 || turn == 2)) {
		projectileAnimation = true;
		if (turn == 1) {
			projectileDirection = camera1->forward;
			projectileDirection.y += 0.2f;
			projectilePosition = player1Position;
		}
		else {
			projectileDirection = camera2->forward;
			projectileDirection.y += 0.4f;
			projectilePosition = player2Position;
		}
	}
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
