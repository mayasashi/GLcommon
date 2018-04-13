//
//  GLcommon.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include "GLcommon_includes.h"
#include "GLcommon_format.h"


typedef enum{
    ERRCHK_SUCCESS,
    ERRCHK_SUSPEND,
    ERRCHK_UNKNOWN
}ERRenum;

struct GLcontext{
    GLuint GL_version_major;
    GLuint GL_version_minor;
    bool ForwardCompatFlg;
    bool CoreProfileFlg;
};

struct windowInfo{
    GLuint width;
    GLuint height;
    char *title;
    GLcontext GLcntxt;
};

struct attribLocation{
    GLuint locator;
    char *str;
};

using uniformLocation = attribLocation;

struct shader{
    const GLuint ID;
    const char *label;
    const GLenum type;
    const char *path;
    GLuint handler;
    std::vector<attribLocation> attribLocation;
    std::vector<uniformLocation> uniformLocation;
    bool compileFlg;
    
    shader(const GLuint init_ID,const char * init_label,const GLenum init_type, const char * init_path) :
        ID(init_ID),
        label(init_label),
        type(init_type),
        path(init_path)
    {}
    
};

struct program{
    const GLuint ID;
    const char *label;
    GLuint handler;
    shader *fragmentShader;
    shader *vertexShader;
    bool linkFlg;
    
    program(const GLuint init_ID,const char *init_label) :
        ID(init_ID),
        label(init_label)
    {}
};

struct tex{
    const GLuint ID;
    const char *label;
	const GLuint width;
	const GLuint height;
    GLuint handler;
    void *srcDataPtr;
    bool minflg;
    bool magflg;
    GLint internalformat;
    GLenum internaltype;
    
    tex(GLuint init_ID,const char *init_label, GLuint init_width, GLuint init_height) :
		ID(init_ID) , 
		label(init_label) , 
		width(init_width), 
		height(init_height) 
	{}
};

class GLcommon {
    
public:
    GLcommon();
    ~GLcommon();
    void createWindow(int width,int height);
    void createVAO();
    void createVBO();
	ERRenum createTexture(GLuint init_ID, const char *init_label, int width, int height);
	ERRenum transferDataToTexture(GLuint init_ID, void *srcData, GLint internalFormat, GLenum internaltype);

    ERRenum createShader(const GLuint init_ID, const char *init_label, const GLenum init_type, const char * init_path);
    ERRenum createProgram(const GLuint init_ID, const char *init_label);
    void flush();
    
private:
	template <typename T> ERRenum checkID(const GLuint ID, const char *func_name, std::vector<T> vec);
	template <typename T> ERRenum checklabel(const char *label, const char *func_name, std::vector<T> vec);
    GLFWwindow *window;
    std::vector<tex*> texturevec;
    std::vector<shader*> shadervec;
    std::vector<program*> programvec;
    
};

GLcommon::GLcommon(){
    if(!glfwInit()){
        std::cout << "failed to initialize glfw environment." << std::endl;
    }
}

GLcommon::~GLcommon(){
    glfwTerminate();
}

template <typename T> 
ERRenum GLcommon::checkID(const GLuint ID, const char *func_name, std::vector<T> vec) {
	bool conflict_flg = false;
	for (typename std::vector<T>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if ((*itr)->ID == ID) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting ID specified.\n",func_name);
		return ERRCHK_SUSPEND;
	}
	return ERRCHK_SUCCESS;
}

template <typename T> 
ERRenum GLcommon::checklabel(const char *label, const char *func_name, std::vector<T> vec) {

	if (strcmp(label, "") == 0 || label == nullptr) {
		printf("SUSPENDED (%s) : label is empty.\n",func_name);
		return ERRCHK_SUSPEND;
	}
	bool conflict_flg = false;
	for (typename std::vector<T>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if (strcmp((*itr)->label, label) == 0) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting label specified.\n", func_name);
		return ERRCHK_SUSPEND;
	}

	return ERRCHK_SUCCESS;
}

ERRenum GLcommon::createProgram(const GLuint init_ID, const char *init_label) {

	if (checkID<program*>(init_ID, __func__, programvec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<program*>(init_label, __func__, programvec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

	program *pr = new program(init_ID, init_label);
	programvec.push_back(pr);

	pr->handler = glCreateProgram();

	return ERRCHK_SUCCESS;
}

ERRenum GLcommon::createShader(const GLuint init_ID, const char *init_label, const GLenum init_type, const char * init_path )
{
	if (checkID<shader*>(init_ID, __func__, shadervec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<shader*>(init_label, __func__, shadervec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

	shader *sh = new shader(init_ID, init_label, init_type, init_path);
	shadervec.push_back(sh);

	sh->handler = glCreateShader(sh->type);
    
    return ERRCHK_SUCCESS;
}

ERRenum GLcommon::createTexture(GLuint init_ID, const char *init_label, int width, int height) {

	if (checkID<tex*>(init_ID, __func__, texturevec) != ERRCHK_SUCCESS) ERRCHK_SUSPEND;
	if (checklabel<tex*>(init_label, __func__, texturevec) != ERRCHK_SUCCESS) ERRCHK_SUSPEND;

    tex* texture = new tex(init_ID,init_label,width,height);
	texturevec.push_back(texture);

	glGenTextures(1, &(texture->handler));

    
    return ERRCHK_SUCCESS;
}

ERRenum GLcommon::transferDataToTexture(GLuint init_ID, void *srcData, GLint internalformat, GLenum internaltype) {
    
    GLenum textureformat = CONVERT(internalformat);
    
    if(textureformat == 0){
        printf("SUSPENDED at transferDataToTexture() : Illegal texture internal format.\n");
        return ERRCHK_SUSPEND;
    }
    
	std::vector<tex*>::iterator itr = texturevec.begin();
	while ((*itr)->ID != init_ID) {
		++itr;
		if (itr == texturevec.end()) {
			printf("SUSPENDED at transferDataToTexure() : Texture not registered.\n");
			return ERRCHK_SUSPEND;
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, (*itr)->handler);

	(*itr)->internalformat = internalformat;
	(*itr)->internaltype = internaltype;


	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, (*itr)->width, (*itr)->height, 0, textureformat, internaltype, srcData);

	glBindTexture(GL_TEXTURE_2D, 0);
	return ERRCHK_SUCCESS;
}


