#pragma once
#include <vector>
#include <vertex.h>
#include <common/shader.h>

class Plane
{
public:
	Plane(float x, float y, float w, float h);
	~Plane();

	void init();
	std::vector<Vertex> getVertices() const;
	std::vector<unsigned int> getIndices() const;
	glm::mat4 getModelMatrix() const;
	void setScale(glm::vec2& scale_);
	void setPosition(glm::vec2& position_);
	void setSize(glm::vec2& size_);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	glm::mat4 modelMatrix;
	void updateModelMatrix();
	float xPos, yPos, width, height;
	unsigned int VAO, VBO, EBO;
	glm::vec2 scale;
	glm::vec2 size;
};