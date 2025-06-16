#include <common/resource_manager.h>

ResourceManager::~ResourceManager()
{
	unloadAllTextures();
	for (auto& shader : shaderPool)
	{
		delete shader;
	}
	shaderPool.clear();
}

Shader* ResourceManager::loadShader(const std::string& v, const std::string& f, bool isShaderFromFile)
{
	Shader* shader = new Shader(v.c_str(), f.c_str(), isShaderFromFile);
	shaderPool.push_back(shader);
	return shader;
}

Texture* ResourceManager::loadTexture(const std::string& filepath)
{
	// Check from texturePool if texture already exists
	auto it = texturePool.find(filepath);
	if (it != texturePool.end()) 
	{
		return it->second;
	}

	Texture* texture = new Texture(filepath.c_str());
	texturePool[filepath] = texture;
	return texture;
}

Texture* ResourceManager::getTexture(const std::string& filepath) const
{
	std::unordered_map<std::string, Texture*>::const_iterator it = texturePool.find(filepath);

	if (it != texturePool.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void ResourceManager::unloadTexture(const std::string& filepath)
{
	std::unordered_map<std::string, Texture*>::iterator it = texturePool.find(filepath);
	if (it != texturePool.end()) 
	{
		delete it->second;
		texturePool.erase(it);
	}
}

void ResourceManager::unloadAllTextures()
{
	for (const std::pair<std::string, Texture*>& pair : texturePool) 
	{
		delete pair.second;
	}
	texturePool.clear();
}
