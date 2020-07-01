#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, glm::vec3 color2, bool fill);
	Mesh* CreateDisk(std::string name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 color2, bool fill);
	Mesh* CreateTriangle(std::string name, glm::vec3 frontCorner, float side, glm::vec3 color,  bool fill);
}

