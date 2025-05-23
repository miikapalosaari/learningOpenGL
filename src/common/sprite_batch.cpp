#include <sprite_batch.h>
#include <glad/gl.h>

SpriteBatch::SpriteBatch()
	:
	VAO(0)
	,VBO(0)
	,EBO(0)
	,currentVertexCount(0)
	,currentIndexCount(0)
	,batchedVertices{}
	,batchedIndices{}
{
	init();
}

SpriteBatch::~SpriteBatch()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
}

void SpriteBatch::addPlane(Plane& plane)
{
	std::vector<Vertex> vertices = plane.getVertices();
	std::vector<GLuint> indices = plane.getIndices();
	glm::mat4 model = plane.getModelMatrix();

	GLuint vertexOffset = currentVertexCount;
	for (auto& index : indices)
	{
		index += vertexOffset;
	}

	std::vector<Vertex> transformedVertices;
	transformedVertices.reserve(vertices.size());

	for (const Vertex& vertex : vertices)
	{
		glm::vec4 transformedPos = model * glm::vec4(vertex.position, 1.0f);
		transformedVertices.push_back({ glm::vec3(transformedPos), vertex.texCoord });
	}

	batchedVertices.insert(batchedVertices.end(), transformedVertices.begin(), transformedVertices.end());
	batchedIndices.insert(batchedIndices.end(), indices.begin(), indices.end());

	currentVertexCount += static_cast<GLuint>(vertices.size());
	currentIndexCount += indices.size();
}

void SpriteBatch::draw(Shader* shader, glm::mat4& projection, glm::mat4& view, unsigned int texID)
{
	updateBuffers();

	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setInt("texture0", 0);

	glActiveTexture(GL_TEXTURE0);
	if (texID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, texID);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, currentIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SpriteBatch::updateBuffers()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(Vertex), batchedVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchedIndices.size() * sizeof(GLuint), batchedIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SpriteBatch::flush()
{
	if (currentIndexCount == 0) return;
	batchedVertices.clear();
	batchedIndices.clear();
	currentVertexCount = 0;
	currentIndexCount = 0;
}

void SpriteBatch::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
