#ifndef RenderingHelpers_h__
#define RenderingHelpers_h__

#include "GraphicsContext.h"
#include "OpenGLState.h"

struct FStencilShadowHelper
{
	struct FDepthPassState
	{
		// Disable stencil test and rendering to frame buffer.
		FDepthPassState() : Stencil(GL_STENCIL_TEST, GL_FALSE)
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_TRUE);
		}

		~FDepthPassState()
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}

	private:
		FScopedGLState Stencil;
	};


	struct FShadowVolumePass // This can be used with either GL_INCR or GL_DECR for increment and decrement pass.
	{
		FShadowVolumePass(GLenum StencilOperation, GLenum FaceCullingMode)
			: Depth(GL_DEPTH_TEST, GL_TRUE), Stencil(GL_STENCIL_TEST, GL_TRUE), Culling(GL_CULL_FACE, GL_TRUE)
		{
			glDepthMask(GL_FALSE);
			glCullFace(FaceCullingMode);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, StencilOperation);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthFunc(GL_LESS);
		}

		~FShadowVolumePass()
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);
		}
	private:
		FScopedGLState Depth;
		FScopedGLState Culling;
		FScopedGLState Stencil;
	};

	struct FShadowedScenePassState
	{
		FShadowedScenePassState() : Culling(GL_CULL_FACE, GL_FALSE), LightingState(GL_LIGHTING, GL_TRUE), Stencil(GL_STENCIL_TEST, GL_TRUE)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		}

		~FShadowedScenePassState()
		{

		}

	private:
		FScopedGLState Culling;
		FScopedGLState LightingState;
		FScopedGLState Stencil;
	};

	struct FLitScenePassState
	{
		FLitScenePassState() : Culling(GL_CULL_FACE, GL_FALSE), LightingState(GL_LIGHTING, GL_TRUE), Stencil(GL_STENCIL_TEST, GL_TRUE)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
			glStencilFunc(GL_EQUAL, 0, 0xFF);
		}

		~FLitScenePassState()
		{

		}

	private:
		FScopedGLState Culling;
		FScopedGLState LightingState;
		FScopedGLState Stencil;
	};
};

struct FReflectionHelper
{
	struct FMaskState
	{
	public:
		FMaskState() : StencilTest(GL_STENCIL_TEST, true), Lighting(GL_LIGHTING, false), DepthTest(GL_DEPTH_TEST, false)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}

		~FMaskState()
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
	private:
		FScopedGLState StencilTest;
		FScopedGLState Lighting;
		FScopedGLState DepthTest;
	};

	struct FMirroredSceneState
	{
	public:
		FMirroredSceneState() : StencilTest(GL_STENCIL_TEST, true), Lighting(GL_LIGHTING, true), DepthTest(GL_DEPTH_TEST, true)
		{
			glStencilFunc(GL_EQUAL, 1, 1);
			glPushMatrix();
			glScalef(1.0f, -1.0f, 1.0f);
		}

		~FMirroredSceneState()
		{
			glPopMatrix();
		}
	private:
		FScopedGLState StencilTest;
		FScopedGLState Lighting;
		FScopedGLState DepthTest;
	};
};

#endif // RenderingHelpers_h__
