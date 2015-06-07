#include "Camera.h"

#include <windows.h>
#include <gl/GLU.h>
#include "glm/gtc/matrix_transform.hpp"

FCamera::FCamera() : Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f)
{
}

FCamera::~FCamera()
{
}

void FCamera::Apply()
{
	glm::vec3 Up = GetUpVector();
	glm::vec3 Forward = Position + GetForwardVector();

	gluLookAt(Position.x, Position.y, Position.z, Forward.x, Forward.y, Forward.z, Up.x, Up.y, Up.z);
}

glm::vec3 FCamera::GetForwardVector()
{
	float cosY = cosf(GetYaw() * 3.1415f / 180.0f);
	float cosP = cosf(GetPitch() * 3.1415f / 180.0f);
	float cosR = cosf(GetRoll() * 3.1415f / 180.0f);
	float sinY = sinf(GetYaw() * 3.1415f / 180.0f);
	float sinP = sinf(GetPitch() * 3.1415f / 180.0f);
	float sinR = sinf(GetRoll() * 3.1415f / 180.0f);

	return glm::vec3(sinY * cosP, sinP, cosP * -cosY);
}

glm::vec3 FCamera::GetRightVector()
{
	return glm::cross(GetForwardVector(), GetUpVector());
}

glm::vec3 FCamera::GetUpVector()
{
	float cosY = cosf(GetYaw() * 3.1415f / 180.0f);
	float cosP = cosf(GetPitch() * 3.1415f / 180.0f);
	float cosR = cosf(GetRoll() * 3.1415f / 180.0f);
	float sinY = sinf(GetYaw() * 3.1415f / 180.0f);
	float sinP = sinf(GetPitch() * 3.1415f / 180.0f);
	float sinR = sinf(GetRoll() * 3.1415f / 180.0f);

	return glm::vec3(-cosY * sinR - sinY * sinP * cosR, cosP * cosR, -sinY * sinR - sinP * cosR * -cosY);
}

glm::mat4 FCamera::GetTransformMatrix()
{
	glm::vec3 Up = GetUpVector();
	glm::vec3 Forward = Position + GetForwardVector();
	return glm::lookAt(Position, Forward, Up);
}