#pragma once
#include <vector>
#include <common/plane.h>

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void addPlane(Plane& plane);
	void draw(Shader* shader, const glm::mat4& projection, const glm::mat4& view, unsigned int texID);
	void flush();

private:
	void init();
	void updateBuffers();
	std::vector<Vertex> batchedVertices;
	std::vector<unsigned int> batchedIndices;
	unsigned int VAO, VBO, EBO;
	unsigned int currentVertexCount;
	unsigned int currentIndexCount;
};