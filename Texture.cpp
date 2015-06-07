#include "Texture.h"
#include "SOIL.h"

FTexture::FTexture(int Width, int Height, uint32_t* Data) : TextureFiltering(Point)
{
	glGenTextures(1, &textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
}

FTexture::FTexture(const std::string& Filename) : TextureFiltering(Point)
{
	textureIndex = SOIL_load_OGL_texture(Filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y);
}

void FTexture::Bind()
{
	GLenum FilteringMode;
	switch (TextureFiltering)
	{
	case Point:
		FilteringMode = GL_NEAREST;
		break;
	case Linear:
		FilteringMode = GL_LINEAR;
		break;
	case Trilinear:
		FilteringMode = GL_LINEAR_MIPMAP_LINEAR;
		break;
	default:
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void FTexture::UploadData(int Width, int Height, uint32_t* Data)
{
	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
}

void FTexture::SetFiltering(ETextureFiltering TextureFiltering)
{
	this->TextureFiltering = TextureFiltering;
}
