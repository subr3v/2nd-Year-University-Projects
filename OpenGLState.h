#ifndef OpenGLState_h__
#define OpenGLState_h__

#include <windows.h>
#include <gl/GL.h>

class FScopedGLState
{
public:
	FScopedGLState(GLenum State, GLboolean bEnable) : State(State), bEnable(bEnable)
	{
		bWasEnabled = GetState();
		if (bEnable != bWasEnabled)
			ChangeState(bEnable);
	}

	~FScopedGLState()
	{
		if (bEnable != bWasEnabled)
			ChangeState(bWasEnabled);
	}
private:
	GLenum State;
	GLboolean bEnable;
	GLboolean bWasEnabled;

	void ChangeState(GLboolean bEnable)
	{
		if (bEnable)
			glEnable(State);
		else
			glDisable(State);
	}

	GLboolean GetState()
	{
		GLboolean bEnabled = false;
		glGetBooleanv(State, &bEnabled);
		return bEnabled;
	}
};

#endif // OpenGLState_h__
