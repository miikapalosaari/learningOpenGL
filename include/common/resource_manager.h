#pragma once
#include <common/texture.h>
#include <common/shader.h>
#include <unordered_map>
#include <vector>
#include <string>

class ResourceManager
{
public:
    ~ResourceManager();
    Shader* loadShader(const std::string& v, const std::string& f, bool isShaderFromFile);
    Texture* loadTexture(const std::string& filepath);
    Texture* getTexture(const std::string& filepath) const;
    void unloadTexture(const std::string& filepath);
    void unloadAllTextures();
private:
	std::unordered_map<std::string, Texture*> texturePool;
    std::vector<Shader*> shaderPool;
};