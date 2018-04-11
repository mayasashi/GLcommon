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
    ERRCHK_SUSPENDED,
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

struct tex{
    const GLuint ID;
    const char *name;
	const GLuint width;
	const GLuint height;
    GLuint handler;
    void *srcDataPtr;
    bool minflg;
    bool magflg;
    GLint internalformat;
    GLenum internaltype;
    
    tex(GLuint init_ID,const char *init_name, GLuint init_width, GLuint init_height) :
		ID(init_ID) , 
		name(init_name) , 
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
	ERRenum createTexture(GLuint init_ID, const char *init_name, int width, int height);
	ERRenum transferDataToTexture(GLuint init_ID, void *srcData, GLint internalFormat, GLenum internaltype);
	ERRenum transferDataToTexture(const char *init_name, void *srcData, GLint internalFormat, GLenum internaltype);
	ERRenum transferDataToTexture(GLuint init_ID, const char *init_name, void *srcData, GLint internalFormat, GLenum internaltype);

    void createShader();
    void createProgram();
    void flush();
    
private:
    GLFWwindow *window;
    std::vector<tex*> texturevec;
	ERRenum transferDataToTextureInternal(GLenum handler);
    
};

GLcommon::GLcommon(){
    if(!glfwInit()){
        std::cout << "failed to initialize glfw environment." << std::endl;
    }
}

GLcommon::~GLcommon(){
    glfwTerminate();
}


ERRenum GLcommon::createTexture(GLuint init_ID, const char *init_name, int width, int height) {
    
    /*Check if name is set properly*/
    if(strcmp(init_name, "") == 0 || init_name == nullptr){
        printf("SUSPENDED : Make sure texture name is not empty and pointer is not nullptr.\n");
        return ERRCHK_SUSPENDED;
    }
    
    /*Check if neither ID nor name conflicts with any of these existing in texture vector.*/
    bool conflict_flg = false;
    for(std::vector<tex*>::iterator itr = texturevec.begin(); itr != texturevec.end(); ++itr)
    {
        if((*itr)->ID == init_ID || strcmp((*itr)->name,init_name) == 0) conflict_flg = true;
    }
    if(conflict_flg){
        printf("SUSPENDED : Make sure neither specified ID nor name conflicts with any of these existing in texture vector.\n");
        return ERRCHK_SUSPENDED;
    }
    
    tex* texture = new tex(init_ID,init_name,width,height);

	glGenTextures(1, &(texture->handler));

    texturevec.push_back(texture);
    
    return ERRCHK_SUCCESS;

}

ERRenum GLcommon::transferDataToTexture(GLuint init_ID, void *srcData, GLint internalformat, GLenum internaltype) {
	std::vector<tex*>::iterator itr = texturevec.begin();
	while ((*itr)->ID != init_ID) {
		++itr;
		if (itr == texturevec.end()) {
			printf("SUSPENDED at transferDataToTexure() : texture not registered.\n");
			return ERRCHK_SUSPENDED;
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, (*itr)->handler);

	(*itr)->internalformat = internalformat;
	(*itr)->internaltype = internaltype;


	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, (*itr)->width, (*itr)->height, 0, GL_RGBA, internaltype, srcData);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ERRCHK_SUCCESS;
}


