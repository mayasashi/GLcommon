#include "GLcommon_includes.h"


GLenum CONVERT(GLenum t) {
	switch (t)
	{
		case GL_R8:                return GL_RED;    break;
		case GL_R8_SNORM:          return GL_RED;    break;
		case GL_R16:               return GL_RED;    break;
		case GL_R16_SNORM:         return GL_RED;    break;
		case GL_RG8:               return GL_RG;     break;
		case GL_RG8_SNORM:         return GL_RG;     break;
		case GL_RG16:              return GL_RG;     break;
		case GL_RG16_SNORM:        return GL_RG;     break;
		case GL_R3_G3_B2:          return GL_RGB;    break;
		case GL_RGB4:              return GL_RGB;    break;
		case GL_RGB5:              return GL_RGB;    break;
		case GL_RGB8:              return GL_RGB;    break;
		case GL_RGB8_SNORM:        return GL_RGB;    break;
		case GL_RGB10:             return GL_RGB;    break;
	default:
		break;
	}
}

