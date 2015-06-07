#include "Lighting.h"

void FPointLight::Apply(int LightIndex)
{
	GLenum Light = GetLightName(LightIndex);
	glEnable(Light);

	glLightfv(Light, GL_AMBIENT, &Ambient[0]);
	glLightfv(Light, GL_DIFFUSE, &Diffuse[0]);
	glLightfv(Light, GL_SPECULAR, &Specular[0]);

	glm::vec4 PosVector = glm::vec4(Position, 1.0f);
	glLightfv(Light, GL_POSITION, &PosVector[0]);
}

glm::vec3 FPointLight::GetLightVector()
{
	return Position;
}

void FDirectionalLight::Apply(int LightIndex)
{
	GLenum Light = GetLightName(LightIndex);
	glEnable(Light);

	glLightfv(Light, GL_AMBIENT, &Ambient[0]);
	glLightfv(Light, GL_DIFFUSE, &Diffuse[0]);
	glLightfv(Light, GL_SPECULAR, &Specular[0]);

	glm::vec4 PosVector = glm::vec4(Direction, 0.0f);
	glLightfv(Light, GL_POSITION, &PosVector[0]);
}

glm::vec3 FDirectionalLight::GetLightVector()
{
	return Direction;
}

void FSpotLight::Apply(int LightIndex)
{
	GLenum Light = GetLightName(LightIndex);
	glEnable(Light);

	glLightfv(Light, GL_AMBIENT, &Ambient[0]);
	glLightfv(Light, GL_DIFFUSE, &Diffuse[0]);
	glLightfv(Light, GL_SPECULAR, &Specular[0]);

	glm::vec4 PosVector = glm::vec4(Position, 1.0f);
	glLightfv(Light, GL_POSITION, &PosVector[0]);
	glLightfv(Light, GL_SPOT_DIRECTION, &Direction[0]);
	glLightf(Light, GL_SPOT_EXPONENT, Exponent);
	glLightf(Light, GL_SPOT_CUTOFF, Cutoff);
}

GLenum ILight::GetLightName(int LightIndex)
{
	return GL_LIGHT0 + LightIndex;
}
