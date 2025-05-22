#pragma once
#include <common/texture.h>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
    ~ResourceManager();
    Texture* loadTexture(const std::string& filepath);
    Texture* getTexture(const std::string& filepath) const;
    void unloadTexture(const std::string& filepath);
    void unloadAllTextures();
private:
	std::unordered_map<std::string, Texture*> texturePool;
};