# 2nd-Year-University-Projects
This repository contains the code for my Graphics Programming and Architectures and Performance modules coursework.

In order to compile the project Autodesk FBX 2014.2.1 must be installed.
Sdk can be downloaded from the following link: http://images.autodesk.com/adsk/files/fbx20142_1_fbxsdk_vs2010_win.exe

In order to run the project from visual studio the folder "bin\put_executable_here" inside executable must be set as working folder.

This project showcases the following rendering techniques:
- 3D animated skeletal models
- Stencil shadow volumes
- Multiple lights
- Animated procedural water (both texture and vertices are animated)
- Procedural transparent spheres and planes
- 4 Different camera control schemes
- User interaction (animated models will be highlighted and will change their current animation when clicked)
	
The project has been developed using OpenGL 1.1 and glm as a library, intel TBB has been used for multi-threading implementation.
	
Instructions:

Press R to reset camera to its original position (camera might start at a strange angle depending on where the mouse is at the start of the application)

Press 1 to 4 to change camera control scheme:
1 - Free fly mode
2 - Free look mode (no wasd movement)
3 - Yaw only mode (you will be locked in the XY plane for movement as well)
4 - Spline mode (the camera will follow the spline indicated by the red curve inside the scene, you will be able to rotate with the mouse as you're moving)

Press H to activate wireframe mode.
Press J to activate shadow volume mode.
