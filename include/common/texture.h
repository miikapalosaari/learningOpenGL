#pragma once
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(const char* filepath);
	~Texture();
	unsigned int getTextureId() const;
	glm::vec2 getSize() const;

private:
	unsigned int texID;
	int width, height;
	glm::vec2 size;
};