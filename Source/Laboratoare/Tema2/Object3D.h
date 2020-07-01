#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object3D
{
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	Mesh* CreateSphere(const char* name, float radius, float stacksCount, float sectorCount);	
	Mesh* CreateCylinder(const char* name, float radius, float length, float stacksCount, float sectorCount, bool deformed);
	void  CreateCylinderEnd(float y, std::vector<VertexFormat>&vertices, std::vector<unsigned short>&ndices,int *index, float radius, float stacksCount);
	Mesh* CreateColoredCylinder(const char* name, float radius, float length, float stacksCount, float sectorCount, bool deformed, glm::vec3 color);
	void  CreateColoredCylinderEnd(float y, std::vector<VertexFormat>& vertices, std::vector<unsigned short>& indices, int* index, float radius, float stacksCount, glm::vec3 color);
}