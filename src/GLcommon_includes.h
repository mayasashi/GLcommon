#ifndef GLCOMMON_INCLUDES
#define GLCOMMON_INCLUDES

#define PLATFORM_MAC

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#ifdef PLATFORM_WIN
#define GLEW_STATIC
#include <gl/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GL/glfw3.h>   //must create folder named GL under usr_local_include or usr_include

//#include <ft2build.h>
//#include FT_FREETYPE_H

#endif
