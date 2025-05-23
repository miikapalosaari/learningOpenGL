#include <common/camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/****************************************************Camera2D****************************************************/
Camera2D::Camera2D(float x, float y, float w, float h)
{
	projection = glm::ortho(x, x + w, y + h, y, -1.0f, 1.0f);
	view = glm::mat4(1.0f);
	position = glm::vec3(0.0f, 0.0f, 3.0f);
}

const glm::mat4& Camera2D::getProjection() const
{
	return projection;
}

const glm::mat4& Camera2D::getView() const
{
	return view;
}

void Camera2D::setPosition(const glm::vec2& p)
{
	position = p;
	view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
}
/****************************************************Camera2D****************************************************/


/****************************************************Camera3D****************************************************/
Camera3D::Camera3D(float fovInDegrees, float w, float h, float near, float far, const glm::vec3& worldUp)
{
	projection = glm::perspective(glm::radians(fovInDegrees), w / h, near, far);
	view = glm::mat4(1.0f);
	position = glm::vec3(0.0f, 0.0f, 3.0f);

	WorldUp = worldUp;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	updateCameraVectors();
}

const glm::mat4& Camera3D::getProjection() const
{
	return projection;
}

const glm::mat4& Camera3D::getView() const
{
	return view;
}

void Camera3D::setLookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	view = glm::lookAt(position, target, up);
}

void Camera3D::setPosition(const glm::vec3& p)
{
	position = p;
}

void Camera3D::processMouseMovement(float offsetX, float offsetY)
{
	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;

	mouseYaw += offsetX;
	mousePitch += offsetY;

	if (mousePitch > 89.0f) mousePitch = 89.0f;
	if (mousePitch < -89.0f) mousePitch = -89.0f;

	updateCameraVectors();
	setLookAt(position, position + Front, Up);
}

void Camera3D::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(mouseYaw)) * cos(glm::radians(mousePitch));
	front.y = sin(glm::radians(mousePitch));
	front.z = sin(glm::radians(mouseYaw)) * cos(glm::radians(mousePitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
/****************************************************Camera3D****************************************************/

