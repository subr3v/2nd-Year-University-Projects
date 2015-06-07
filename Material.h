#ifndef Material_h__
#define Material_h__

#include <string>
#include <glm/glm.hpp>

class FMaterial
{
public:
	FMaterial(const std::string& TexturePath);
	FMaterial(class FTexture* Texture);
	FMaterial();
	~FMaterial();

	void Apply();

	void SetAmbient(const glm::vec4& Ambient) { this->Ambient = Ambient; }
	void SetDiffuse(const glm::vec4& Diffuse) { this->Diffuse = Diffuse; }
	void SetEmission(const glm::vec4& Emission) { this->Emission = Emission; }
	void SetSpecular(const glm::vec4& Specular) { this->Specular = Specular; }
	void SetShininess(float Shininess) { this->Shininess = Shininess; }

private:
	glm::vec4 Ambient, Diffuse, Emission, Specular;
	float Shininess;
	class FTexture* Texture;
};

#endif // Material_h__
