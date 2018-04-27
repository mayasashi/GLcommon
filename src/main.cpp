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

    com->Draw([](){
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });

    return 0;
}
