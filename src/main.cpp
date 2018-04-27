//
//  main.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/04.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include "GLcommon.h"

int main(int argc, const char * argv[]) {
	
	GLcommon * com = new GLcommon();
    
	com->createWindowandMakeContext(128, 128);
    
    com->VBO_Create(34, "fd");
    com->VBO_StoreEmpty(34, sizeof(int), GL_STATIC_DRAW);
    
    com->VBO_Create(35, "fs");
    com->VBO_StoreEmpty(35, sizeof(int), GL_STATIC_DRAW);

    com->Draw([](){
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });

    return 0;
}
