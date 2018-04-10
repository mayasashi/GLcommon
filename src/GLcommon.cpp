//
//  GLcommon.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <GL/glfw3.h>   //must create folder named GL under usr_local_include or usr_include
#include <OpenGL/gl3.h>

typedef struct{
    GLuint GL_version_major;
    GLuint GL_version_minor;
    bool ForwardCompatFlg;
    bool CoreProfileFlg;
}GLcontext;

typedef struct{
    GLuint width;
    GLuint height;
    char *title;
    GLcontext GLcntxt;
}windowInfo;

typedef struct{
    bool minflg;
    bool magflg;
    GLuint width;
    GLuint height;
}texInfo;

typedef struct{
    GLuint handler;
    void *srcDataPtr;
    texInfo &info;
}tex;

class GLcommon {
    GLcommon();
    ~GLcommon();
    void createWindow(int width,int height);
    void createVAO();
    void createVBO();
    void createTexture();
    void createShader();
    void createProgram();
    void flush();
    
public:
    GLFWwindow *window;
    std::vector<tex> texturevec;
    
};

GLcommon::GLcommon(){
    if(!glfwInit()){
        std::cout << "failed to initialize glfw environment." << std::endl;
    }
}
GLcommon::~GLcommon(){
    glfwTerminate();
}


void GLcommon::createTexture(){
    
}
