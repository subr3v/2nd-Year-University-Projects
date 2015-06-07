#include "Material.h"
#include "Texture.h"
#include "GraphicsContext.h"


FMaterial::FMaterial(const std::string& TexturePath) : Ambient(1.0f), Diffuse(1.0f), Emission(0.0f), Shininess(0), Specular(1.0f)
{
	Texture = new FTexture(TexturePath);
}

FMaterial::FMaterial() : Ambient(1.0f), Diffuse(1.0f), Emission(0.0f), Shininess(0), Specular(1.0f)
{
	Texture = nullptr;
}

FMaterial::FMaterial(FTexture* Texture) : Ambient(1.0f), Diffuse(1.0f), Emission(0.0f), Shininess(0), Specular(1.0f), Texture(Texture)
{

}

FMaterial::~FMaterial()
{
	delete Texture;
}

void FMaterial::Apply()
{
	if (Texture != nullptr)
		Texture->Bind();
	else
		glDisable(GL_TEXTURE_2D);

	glColor4f(Diffuse.r, Diffuse.g, Diffuse.b, Diffuse.a); // This ensures that things drawn without lighting are affected by diffuse colour anyways.

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &Ambient[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &Diffuse[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &Specular[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &Emission[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
}
