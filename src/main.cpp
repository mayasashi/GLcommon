//
//  main.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/04.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include "GLcommon.h"
#include <CL/cl.h>

inline int print() {
#ifdef PLATFORM_WIN
	char test[10];
	printf("Press any key and enter to continue.\n");
	fgets(test, 10, stdin);

#endif
	return 0;
}


int main(int argc, const char * argv[]) {

	cl_int err;
	cl_uint platformNum;
	err = clGetPlatformIDs(0, NULL, &platformNum);
	if (err != CL_SUCCESS) {
		printf("Error calling clGetPlatformIDs. Error code: %d\n", err);
		return -1;
	}
	
	printf("Detected OpenCL platforms: %d\n", platformNum);

	enum GL_ID {
		ID_NULL,
		ID_VBO_POS,
		ID_VAO,
		ID_PROGRAM,
		ID_SHADER_VERTEX_TRIANGLE,
		ID_SHADER_FRAGMENT_TRIANGLE
	};
	
	GLcommon * com = new GLcommon();
    
	com->createWindowandMakeContext(128, 128);
    
	float position[9] = {
		-1.0f,-1.0f,0.0f,
		 1.0f,-1.0f,0.0f,
		 0.0f, 1.0f,0.0f
	};

	com->VBO_Create(ID_VBO_POS, "vbo_pos");
	com->VBO_StoreData(ID_VBO_POS, 3, 3, GL_FLOAT, GL_STATIC_DRAW, GL_FALSE, 0, sizeof(float)*9, position);


	com->Shader_Create(ID_SHADER_VERTEX_TRIANGLE, "vshader_triangle", GL_VERTEX_SHADER, "..\\..\\..\\src\\shader\\001.vs");
	com->Shader_Create(ID_SHADER_FRAGMENT_TRIANGLE, "fshader_triangle", GL_FRAGMENT_SHADER, "..\\..\\..\\src\\shader\\001.fs");

	com->Shader_AddAttribLocation(ID_SHADER_VERTEX_TRIANGLE, "pos", 1);

	com->Program_Create(ID_PROGRAM, "prog_triangle");
	com->Program_AttachShader(ID_PROGRAM, ID_SHADER_VERTEX_TRIANGLE);
	com->Program_AttachShader(ID_PROGRAM, ID_SHADER_FRAGMENT_TRIANGLE);
	com->Program_LinkShader(ID_PROGRAM);

	com->VAO_Create(ID_VAO, "vao_triangle");
	com->VAO_VertexAttribArray_Register(ID_VAO, ID_VBO_POS, ID_SHADER_VERTEX_TRIANGLE, 1);

	printf("%d\n", sizeof((void *)position));

    com->Draw([&](){

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		com->Program_USE(ID_PROGRAM, [&]() {
			com->VAO_USE(ID_VAO, [&]() {
				glDrawArrays(GL_TRIANGLES, 0, 3);
			});
		});

    });

	delete com;

    return print();
}
