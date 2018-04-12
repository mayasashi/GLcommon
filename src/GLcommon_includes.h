#ifndef GLCOMMON_INCLUDES
#define GLCOMMON_INCLUDES

#define PLATFORM_WIN

#include <iostream>
#include <vector>

#ifdef PLATFORM_WIN
#define GLEW_STATIC
#include <gl/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GL/glfw3.h>   //must create folder named GL under usr_local_include or usr_include

#endif
