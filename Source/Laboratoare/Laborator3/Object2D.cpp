#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{	
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateDisk(std::string name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 color2, bool fill)
{
	std::vector<unsigned short> indices;
	std::vector<VertexFormat> vertices;
	int index = 0;
	for (double i = 0; i < 2 * AI_MATH_PI_F; i += 0.001)
	{	
		if(i < 0.003)
			vertices.push_back(VertexFormat(glm::vec3(center.x + radius * cos(i), center.y + radius * sin(i), 0), color2));
		else
			vertices.push_back(VertexFormat(glm::vec3(center.x + radius * cos(i), center.y + radius * sin(i), 0), color));
		indices.push_back(index);
		index++;
	}
	indices.push_back(0);

	Mesh* disk = new Mesh(name);
	disk->SetDrawMode(GL_TRIANGLE_FAN);

	disk->InitFromData(vertices, indices);
	return disk;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 frontCorner, float side, glm::vec3 color, bool fill)
{
	glm::vec3 corner = frontCorner;

	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(frontCorner, color));
	vertices.push_back(VertexFormat(glm::vec3(frontCorner.x - side, frontCorner.y + side, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(frontCorner.x - side, frontCorner.y - side, 0), color));
	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2};

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, glm::vec3 color2 ,bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color2),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),  //width
		VertexFormat(corner + glm::vec3(length, width, 0), color), //witdh, length
		VertexFormat(corner + glm::vec3(0, width, 0), color2)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}