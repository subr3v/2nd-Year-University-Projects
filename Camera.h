#ifndef Camera_h__
#define Camera_h__

#include <glm/glm.hpp>

class FCamera
{
public:
	FCamera();
	~FCamera();

	void SetPosition(const glm::vec3& Position) { this->Position = Position; }
	void SetRotation(const glm::vec3& Rotation) { this->Rotation = Rotation; } // Rotation represented in Euler form.

	glm::vec3 GetRotation() { return Rotation; }

	float GetYaw() { return Rotation.y; }
	float GetPitch() { return Rotation.x; }
	float GetRoll() { return Rotation.z; }

	void SetYaw(float Yaw) { Rotation.y = Yaw; }
	void SetPitch(float Pitch) { Rotation.x = Pitch; }
	void SetRoll(float Roll) { Rotation.z = Roll; }

	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();

	glm::vec3 GetPosition() { return Position; }

	glm::mat4 GetTransformMatrix();

	void Apply();
private:
	glm::vec3 Position;
	glm::vec3 Rotation;
};

#endif // Camera_h__
