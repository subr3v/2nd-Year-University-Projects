#ifndef Lighting_h__
#define Lighting_h__


#include <windows.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include "SceneNode.h"

class ILight
{
public:
	bool bIsActive;
	bool bCastShadow;

	glm::vec4 Ambient;
	glm::vec4 Diffuse;
	glm::vec4 Specular;

	ILight(const glm::vec4& Diffuse) : bIsActive(true), bCastShadow(true), Ambient(0.0f), Diffuse(Diffuse), Specular(0.0f) { }
	virtual void Apply(int LightIndex) = 0;

	virtual glm::vec3 GetLightVector() { return glm::vec3(0.0f, 0.0f, 0.0f); }
	virtual bool IsPointSource() { return false; }

protected:
	GLenum GetLightName(int LightIndex);
};

class FPointLight : public ILight
{
public:
	glm::vec3 Position;

	FPointLight(const glm::vec4& Diffuse, const glm::vec3& Position) : ILight(Diffuse), Position(Position) { }
	void Apply(int LightIndex) override;
	virtual glm::vec3 GetLightVector() override;
	virtual bool IsPointSource() override { return true; }
};

class FDirectionalLight : public ILight
{
public:
	glm::vec3 Direction;

	FDirectionalLight(const glm::vec4& Diffuse, const glm::vec3& Direction) : ILight(Diffuse), Direction(Direction) { }
	void Apply(int LightIndex) override;
	virtual glm::vec3 GetLightVector() override;
	virtual bool IsPointSource() override { return false; }
};

class FSpotLight : public ILight
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;
	float Cutoff;
	float Exponent;

	FSpotLight(const glm::vec4& Diffuse, const glm::vec3& Position, const glm::vec3& Direction, float Cutoff, float Exponent) : ILight(Diffuse), Position(Position), Direction(Direction), Cutoff(Cutoff), Exponent(Exponent) { }
	void Apply(int LightIndex) override;
};

class FLightSceneNode : public FSceneNode
{
public:
	FLightSceneNode(FPointLight* Light) : Light(Light) { }

	virtual void OnUpdate(float DeltaTime) override
	{
		Light->Position = glm::vec3(GetGlobalTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));;
	}

private:
	FPointLight* Light;
};

#endif // Lighting_h__
