//
//  GLcommon.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//
#define PLATFORM_MAC


#include <iostream>
#include <vector>

#ifdef PLATFORM_WIN
#include <gl/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GL/glfw3.h>   //must create folder named GL under usr_local_include or usr_include

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
    GLenum handler;
    void *srcDataPtr;
    bool minflg;
    bool magflg;
    GLuint width;
    GLuint height;
    GLint internalformat;
    GLenum internaltype;
    
    tex(GLuint init_ID,const char *init_name) : ID(init_ID) , name(init_name) {}
};

class GLcommon {
    
public:
    GLcommon();
    ~GLcommon();
    void createWindow(int width,int height);
    void createVAO();
    void createVBO();
    ERRenum createTexture(GLuint init_ID,const char *init_name);
    void createShader();
    void createProgram();
    void flush();
    
private:
    GLFWwindow *window;
    std::vector<tex*> texturevec;
    
};

GLcommon::GLcommon(){
    if(!glfwInit()){
        std::cout << "failed to initialize glfw environment." << std::endl;
    }
}

GLcommon::~GLcommon(){
    glfwTerminate();
}


ERRenum GLcommon::createTexture(GLuint init_ID,const char *init_name){
    
    /*Check if name is set properly*/
    if(strcmp(init_name, "") == 0 || init_name == nullptr){
        printf("SUSPENDED : Make sure texture name is not empty and pointer is not nullptr.\n");
        return ERRCHK_SUSPENDED;
    }
    
    /*Check if neither ID nor name do not conflict with any of these existing in texture vector.*/
    bool conflict_flg = false;
    for(std::vector<tex*>::iterator itr = texturevec.begin(); itr != texturevec.end(); ++itr)
    {
        if((*itr)->ID == init_ID || strcmp((*itr)->name,init_name) == 0) conflict_flg = true;
    }
    if(conflict_flg){
        printf("SUSPENDED : Make sure neither specified ID nor name do not conflict with any of these existing in texture vector.\n");
        return ERRCHK_SUSPENDED;
    }
    
    tex* texture = new tex(init_ID,init_name);
    
    texturevec.push_back(texture);
    
    return ERRCHK_SUCCESS;
}
