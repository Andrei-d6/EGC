#include "Object3D.h"

using namespace std;
#include <Core/Engine.h>
#include <iostream>
#include <string>
#include <vector>

Mesh* Object3D::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh* mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	mesh->vertices = vertices;
	mesh->indices = indices;
	return mesh;
}

Mesh* Object3D::CreateSphere(const char* name, float radius, float stacksCount, float sectorCount)
{
	std::vector<VertexFormat>vertices;
	float x, y, z, xy;
	float nx, ny, nz, lengthInv = 1.0f / radius;
	float s, t;

	float sectorStep = 2.0f * AI_MATH_PI / sectorCount;
	float stackStep = AI_MATH_PI / stacksCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacksCount; i++)
	{
		stackAngle = AI_MATH_PI / 2.0f - i * stackStep;
		xy = radius * cos(stackAngle);
		z = radius * sin(stackAngle);

		for (int j = 0; j <= sectorCount; j++)
		{
			sectorAngle = j * sectorStep;

			x = xy * cos(sectorAngle);
			y = xy * sin(sectorAngle);

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			s = (float)j / sectorCount;
			t = (float)i / stacksCount;
			vertices.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec3(1, 1, 1),  glm::vec2(s, t)));
		}
	}

	std::vector<unsigned short>indices;
	int k1, k2;

	for (int i = 0; i < stacksCount; i++)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
				

			if(i != (stacksCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return CreateMesh(name, vertices, indices);
}

Mesh* Object3D::CreateCylinder(const char* name, float radius, float length, float stacksCount, float sectorCount, bool deformed)
{	
	std::vector<VertexFormat>vertices;
	std::vector<VertexFormat>normals;
	std::vector<unsigned short>indices;

	float m_radius = radius;
	float twoPi = 2.0f * AI_MATH_PI;

	// The side of the cylinder
	const float dTheta = twoPi / static_cast<float>(stacksCount);
	const float dy = length / static_cast<float>(sectorCount - 1);

	// Iterate over heights (rings)
	int index = 0;

	for (int ring = 0; ring < sectorCount; ++ring)
	{
		const float y = -length / 2.0f + static_cast<float>(ring)* dy;

		// Iterate over slices (segments in a ring)
		for (int slice = 0; slice < stacksCount + 1; ++slice)
		{
			const float theta = static_cast<float>(slice)* dTheta;
			const float cosTheta = cosf(theta);
			const float sinTheta = sinf(theta);

			float randomOffset = rand() % 10 / 20.0f;
			randomOffset /= 4.0f;

			if (rand() % 2)
				randomOffset *= -1;

			if (deformed == false)
				randomOffset = 0;

			float R, G, B;
			//Start R = 137 G = 168 B = 230
			//End   R = 82  G = 120 B = 204

			//Start R = 130 G = 170 B = 250
			//End   R = 80  G = 120 B = 200
			
			G = 120;
			B = 200;

			float offset = 0.0f;

			do {
				R = rand() % 131;
			} while (R < 80);

			offset = R - 80;
			G += offset;
			B += offset;
			
			R /= 255.0f; G /= 255.0f; B /= 255.0f;

			vertices.push_back(VertexFormat(glm::vec3(m_radius * cosTheta, y, m_radius * sinTheta + randomOffset),
				glm::vec3(cosTheta, 0.0f, sinTheta),
				glm::vec3(R, G, B)));
			
			index += 3;
		}
	}

	int elIndex = 0;
	for (int i = 0; i < sectorCount - 1; ++i)
	{
		const int ringStartIndex = i * (stacksCount + 1);
		const int nextRingStartIndex = (i + 1) * (stacksCount + 1);

		for (int j = 0; j <= stacksCount; ++j)
		{
			indices.push_back(ringStartIndex + j);
			indices.push_back(ringStartIndex + j + 1);
			indices.push_back(nextRingStartIndex + j);
			indices.push_back(nextRingStartIndex + j);
			indices.push_back(ringStartIndex + j + 1);
			indices.push_back(nextRingStartIndex + j + 1);
		}
	}

	// Generate the vertex data and indices for the two end caps
	CreateCylinderEnd(length / 2.0f, vertices, indices,&index, radius, stacksCount);
	CreateCylinderEnd( (-length) / 2.0f, vertices, indices, &index, radius, stacksCount);

	return CreateMesh(name, vertices, indices);
}

void Object3D::CreateCylinderEnd(float y, std::vector<VertexFormat> &vertices, std::vector<unsigned short>&indices, int *index, float radius, float stacksCount)
{	
	int endCapStartIndex = (*index) / 3;

	vertices.push_back(VertexFormat(glm::vec3(0.0f, y, 0.0f),
		glm::vec3(0.0f, (y >= 0.0f) ? 1.0f : -1.0, 0.0f),
		glm::vec3((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100)));

	(*index) += 3;
	const float dTheta = (2.0f * AI_MATH_PI) / static_cast<float>(stacksCount);
	

	for (int slice = 0; slice < stacksCount; ++slice)
	{
		const float theta = static_cast<float>(slice)* dTheta;
		const float cosTheta = cosf(theta);
		const float sinTheta = sinf(theta);

		vertices.push_back(VertexFormat(glm::vec3(radius * cosTheta, y, radius * sinTheta),
			glm::vec3(0.0f, (y >= 0.0f) ? 1.0f : -1.0, 0.0f),
			glm::vec3((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100)));

		(*index) += 3;
	}

	for (int i = 0; i < stacksCount; ++i)
	{
		// Use center point and i+1, and i+2 vertices to create triangles
		if (i != stacksCount - 1)
		{	
			indices.push_back(endCapStartIndex);
			indices.push_back(endCapStartIndex + i + 1);
			indices.push_back(endCapStartIndex + i + 2);

		}
		else
		{	
			indices.push_back(endCapStartIndex);
			indices.push_back(endCapStartIndex + i + 1);
			indices.push_back(endCapStartIndex + 1);
		}
	}
}


Mesh* Object3D::CreateColoredCylinder(const char* name, float radius, float length, float stacksCount, float sectorCount, bool deformed, glm::vec3 color)
{
	std::vector<VertexFormat>vertices;
	std::vector<VertexFormat>normals;
	std::vector<unsigned short>indices;

	float m_radius = radius;
	float twoPi = 2.0f * AI_MATH_PI;

	// The side of the cylinder
	const float dTheta = twoPi / static_cast<float>(stacksCount);
	const float dy = length / static_cast<float>(sectorCount - 1);

	// Iterate over heights (rings)
	int index = 0;

	for (int ring = 0; ring < sectorCount; ++ring)
	{
		const float y = -length / 2.0f + static_cast<float>(ring)* dy;

		// Iterate over slices (segments in a ring)
		for (int slice = 0; slice < stacksCount + 1; ++slice)
		{
			const float theta = static_cast<float>(slice)* dTheta;
			const float cosTheta = cosf(theta);
			const float sinTheta = sinf(theta);

			float randomOffset = rand() % 10 / 20.0f;
			randomOffset /= 4.0f;

			if (rand() % 2)
				randomOffset *= -1;

			if (deformed == false)
				randomOffset = 0;


			vertices.push_back(VertexFormat(glm::vec3(m_radius * cosTheta, y, m_radius * sinTheta + randomOffset),
				glm::vec3(cosTheta, 0.0f, sinTheta),
				color));

			index += 3;
		}
	}

	int elIndex = 0;
	for (int i = 0; i < sectorCount - 1; ++i)
	{
		const int ringStartIndex = i * (stacksCount + 1);
		const int nextRingStartIndex = (i + 1) * (stacksCount + 1);

		for (int j = 0; j <= stacksCount; ++j)
		{
			indices.push_back(ringStartIndex + j);
			indices.push_back(ringStartIndex + j + 1);
			indices.push_back(nextRingStartIndex + j);
			indices.push_back(nextRingStartIndex + j);
			indices.push_back(ringStartIndex + j + 1);
			indices.push_back(nextRingStartIndex + j + 1);
		}
	}

	// Generate the vertex data and indices for the two end caps
	CreateColoredCylinderEnd(length / 2.0f, vertices, indices, &index, radius, stacksCount, color);
	CreateColoredCylinderEnd((-length) / 2.0f, vertices, indices, &index, radius, stacksCount, color);

	return CreateMesh(name, vertices, indices);
}

void Object3D::CreateColoredCylinderEnd(float y, std::vector<VertexFormat>& vertices, std::vector<unsigned short>& indices, int* index, float radius, float stacksCount, glm::vec3 color)
{
	int endCapStartIndex = (*index) / 3;

	vertices.push_back(VertexFormat(glm::vec3(0.0f, y, 0.0f),
		glm::vec3(0.0f, (y >= 0.0f) ? 1.0f : -1.0, 0.0f),
		color));

	(*index) += 3;
	const float dTheta = (2.0f * AI_MATH_PI) / static_cast<float>(stacksCount);


	for (int slice = 0; slice < stacksCount; ++slice)
	{
		const float theta = static_cast<float>(slice)* dTheta;
		const float cosTheta = cosf(theta);
		const float sinTheta = sinf(theta);

		vertices.push_back(VertexFormat(glm::vec3(radius * cosTheta, y, radius * sinTheta),
			glm::vec3(0.0f, (y >= 0.0f) ? 1.0f : -1.0, 0.0f),
			color));

		(*index) += 3;
	}

	for (int i = 0; i < stacksCount; ++i)
	{
		// Use center point and i+1, and i+2 vertices to create triangles
		if (i != stacksCount - 1)
		{
			indices.push_back(endCapStartIndex);
			indices.push_back(endCapStartIndex + i + 1);
			indices.push_back(endCapStartIndex + i + 2);

		}
		else
		{
			indices.push_back(endCapStartIndex);
			indices.push_back(endCapStartIndex + i + 1);
			indices.push_back(endCapStartIndex + 1);
		}
	}
}