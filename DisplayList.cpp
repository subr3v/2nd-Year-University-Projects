#include "DisplayList.h"

FOpenGLDisplayList::FOpenGLDisplayList()
{
	Handle = glGenLists(1);
}

FOpenGLDisplayList::~FOpenGLDisplayList()
{
	glDeleteLists(Handle, 1);
}

void FOpenGLDisplayList::Begin()
{
	glNewList(Handle, GL_COMPILE);
}

void FOpenGLDisplayList::End()
{
	glEndList();
}

void FOpenGLDisplayList::Render()
{
	glCallList(Handle);
}