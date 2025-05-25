#include <common/plane.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

Plane::Plane(float x, float y, float w, float h)
	: 
	 xPos(x) 
	,yPos(y) 
	,width(w)
	,height(h)
	,modelMatrix(1.0f)
	,VAO(0)
	,VBO(0)
	,EBO(0)
	,scale({1.0f, 1.0f})
	,size({32.0f, 32.0f})
{
	updateModelMatrix();
	init();
}

Plane::~Plane()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void Plane::init()
{
	vertices =
	{
		{glm::vec3(xPos, yPos, 0.0f),					glm::vec2(0.0f, 0.0f)},
		{glm::vec3(xPos + width, yPos, 0.0f),			glm::vec2(1.0f, 0.0f)},
		{glm::vec3(xPos, yPos +	height, 0.0f),			glm::vec2(0.0f, 1.0f)},
		{glm::vec3(xPos + width, yPos + height, 0.0f),	glm::vec2(1.0f, 1.0f)},
	};

	indices = { 0, 1, 2, 1, 3, 2 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

std::vector<Vertex> Plane::getVertices() const
{
	return vertices;
}

std::vector<unsigned int> Plane::getIndices() const 
{
	return indices;
}

glm::mat4 Plane::getModelMatrix() const
{
	return modelMatrix;
}

void Plane::setScale(glm::vec2& scale_)
{
	scale = scale_;
	updateModelMatrix();
}

void Plane::setPosition(glm::vec2& position_)
{
	xPos = position_.x;
	yPos = position_.y;
	updateModelMatrix();
}

void Plane::setSize(glm::vec2& size_)
{
	size = size_;

	vertices[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
	vertices[1].position = glm::vec3(size.x, 0.0f, 0.0f);
	vertices[2].position = glm::vec3(0.0f, size.y, 0.0f);
	vertices[3].position = glm::vec3(size.x, size.y, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Plane::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Plane::getVAO() const
{
	return VAO;
}

void Plane::updateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 1.0f));
}
