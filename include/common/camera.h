#pragma once
#include <glm/glm.hpp>
class Camera3D
{
public:
	Camera3D(float fovInDegrees, float w, float h, float near, float far, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;

	void setLookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);
	void setPosition(const glm::vec3& p);
	void processMouseMovement(float offsetX, float offsetY);
	void updateCameraVectors();

private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 position;

	glm::vec3 Front, Right, Up, WorldUp;

	bool firstMouse = true;
	float mouseYaw = -90.0f;
	float mousePitch = 0.0f;
	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
	float mouseSensitivity = 0.1f;
};

class Camera2D
{
public:
	Camera2D(float x, float y, float w, float h);
	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;

private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 position;
};