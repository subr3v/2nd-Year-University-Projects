#ifndef DisplayList_h__
#define DisplayList_h__

#include <windows.h>
#include <gl/GL.h>

class FOpenGLDisplayList
{
public:
	FOpenGLDisplayList();
	~FOpenGLDisplayList();

	void Begin();
	void End();

	void Render();
private:
	GLuint Handle;
};

#endif // DisplayList_h__
