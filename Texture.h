#ifndef Texture_h__
#define Texture_h__

#include <string>
#include <windows.h>
#include <gl/GL.h>
#include <cstdint>

enum ETextureFiltering
{
	Point,
	Linear,
	Trilinear,
};

class FTexture
{
public:
	FTexture(int Width, int Height, uint32_t* Data);
	FTexture(const std::string& Filename);

	void Bind();
	void SetFiltering(ETextureFiltering TextureFiltering);
	void UploadData(int Width, int Height, uint32_t* Data);
private:
	GLuint textureIndex;
	ETextureFiltering TextureFiltering;
};

#endif // Texture_h__
