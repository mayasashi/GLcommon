#include "GLcommon_includes.h"


GLenum CONVERT(GLint t) {
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
        case GL_RGB12:             return GL_RGB;    break;
        case GL_RGB16_SNORM:       return GL_RGB;    break;
        case GL_RGBA2:             return GL_RGB;    break;
        case GL_RGBA4:             return GL_RGB;    break;
        case GL_RGB5_A1:           return GL_RGBA;   break;
        case GL_RGBA8:             return GL_RGBA;   break;
        case GL_RGBA8_SNORM:       return GL_RGBA;   break;
        case GL_RGB10_A2:          return GL_RGBA;   break;
        case GL_RGB10_A2UI:        return GL_RGBA;   break;
        case GL_RGBA12:            return GL_RGBA;   break;
        case GL_RGBA16:            return GL_RGBA;   break;
        case GL_SRGB8:             return GL_RGB;    break;
        case GL_SRGB8_ALPHA8:      return GL_RGBA;   break;
        case GL_R16F:              return GL_RED;    break;
        case GL_RG16F:             return GL_RG;     break;
        case GL_RGB16F:            return GL_RGB;    break;
        case GL_RGBA16F:           return GL_RGBA;   break;
        case GL_R32F:              return GL_RED;    break;
        case GL_RG32F:             return GL_RG;     break;
        case GL_RGB32F:            return GL_RGB;    break;
        case GL_RGBA32F:           return GL_RGBA;   break;
        case GL_R11F_G11F_B10F:    return GL_RGB;    break;
        case GL_RGB9_E5:           return GL_RGB;    break;
        case GL_R8I:               return GL_RED;    break;
        case GL_R8UI:              return GL_RED;    break;
        case GL_R16I:              return GL_RED;    break;
        case GL_R16UI:             return GL_RED;    break;
        case GL_R32I:              return GL_RED;    break;
        case GL_R32UI:             return GL_RED;    break;
        case GL_RG8I:              return GL_RG;     break;
        case GL_RG8UI:             return GL_RG;     break;
        case GL_RG16I:             return GL_RG;     break;
        case GL_RG16UI:            return GL_RG;     break;
        case GL_RG32I:             return GL_RG;     break;
        case GL_RG32UI:            return GL_RG;     break;
        case GL_RGB8I:             return GL_RGB;    break;
        case GL_RGB8UI:            return GL_RGB;    break;
        case GL_RGB16I:            return GL_RGB;    break;
        case GL_RGB16UI:           return GL_RGB;    break;
        case GL_RGB32I:            return GL_RGB;    break;
        case GL_RGB32UI:           return GL_RGB;    break;
        case GL_RGBA8I:            return GL_RGBA;   break;
        case GL_RGBA8UI:           return GL_RGBA;   break;
        case GL_RGBA16I:           return GL_RGBA;   break;
        case GL_RGBA16UI:          return GL_RGBA;   break;
        case GL_RGBA32I:           return GL_RGBA;   break;
        case GL_RGBA32UI:          return GL_RGBA;   break;
        
	default:
		break;
	}
    
    return 0;
}

