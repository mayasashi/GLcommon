//
//  GLcommon.h
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#ifndef GLcommon_h
#define GLcommon_h

#include <GL/glfw3.h>   //must create folder named GL under usr_local_include or usr_include
#include <OpenGL/gl3.h>
#include <vector>

class GLcommon {
    GLcommon(){};
    ~GLcommon(){};
    void createWindow(int width,int height);
    void loadVAO();
    void loadVBO();
    void loadTexture();
    void loadProgramShader();
    void flush();
    
    
    
public:
    GLFWwindow window;
    
};


#endif /* GLcommon_h */
