//
//  main.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/04.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include "GLcommon_includes.h"

int main(int argc, const char * argv[]) {
	
	GLcommon * com = new GLcommon();

	com->createWindowandMakeContext(640, 480);

	do {
		glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		com->flush();
	} while (com->closeflg());

    return 0;
}
