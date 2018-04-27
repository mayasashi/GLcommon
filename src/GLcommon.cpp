//
//  GLcommon.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include "GLcommon.h"
#include "GLcommon_format.h"

#define ERRCHECK() \
    if(!errchk) return EXIT(ERRCHK_SKIP); \
    if(!checkContextFlg(__func__)) return EXIT(ERRCHK_CONTEXT_ABORT);

GLcommon::GLcommon(){
    if(!glfwInit()){
        std::cout << "failed to initialize glfw environment." << std::endl;
        
    }
    errchk = true;
    contextFlg = false;
}

GLcommon::~GLcommon(){
    glfwTerminate();
}

template <typename T> 
ERRenum GLcommon::checkID(GLuint ID, const char *func_name, const std::vector<T>& vec) {
	bool conflict_flg = false;
	for (typename std::vector<T>::const_iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if ((*itr)->ID == ID) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting ID specified.\n",func_name);
        
		return EXIT(ERRCHK_SUSPEND);
	}
	return EXIT(ERRCHK_SUCCESS);
}

template <typename T> ERRenum GLcommon::findID(GLuint ID, const char *func_name, const std::vector<T>& vec, GLuint &index)
{
	typename std::vector<T>::const_iterator itr = vec.begin();
    GLuint count = 0;
    while(itr != vec.end() && (*itr)->ID != ID){
        ++itr;
        ++count;
    }
    if(itr != vec.end() && (*itr)->ID == ID){
        index = count;
        return EXIT(ERRCHK_SUCCESS);
    }else{
        printf("SUSPENDED (%s) : ID not found.\n",func_name);
        
        return EXIT(ERRCHK_SUSPEND);
    }
	
}

template <typename T> 
ERRenum GLcommon::checklabel(const char *label, const char *func_name, const std::vector<T>& vec) {

	if (strcmp(label, "") == 0 || label == nullptr) {
		printf("SUSPENDED (%s) : label is empty.\n",func_name);
        
		return EXIT(ERRCHK_SUSPEND);
	}
	bool conflict_flg = false;
	for (typename std::vector<T>::const_iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if (strcmp((*itr)->label, label) == 0) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting label specified.\n", func_name);
        
		return EXIT(ERRCHK_SUSPEND);
	}

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::createWindowandMakeContext(unsigned short width, unsigned short height)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
	if (window != NULL) {
		printf("SUSPENDED (%s) : GLFW window already exists.\n");
		return EXIT(ERRCHK_SUSPEND);
	}
	if (width == 0 || height == 0) {
		printf("SUSPENDED (%s) : Value of window's width or height should be greater than zero.\n");
		return EXIT(ERRCHK_SUSPEND);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	window = glfwCreateWindow(width, height, "TITLE", NULL, NULL);
	if (window == NULL) {
		printf("SUSPENDED (%s) : failed to create window.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}

	glfwMakeContextCurrent(window);

#ifdef PLATFORM_WIN
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("SUSPENDED (%s) : failed to initialize glew.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}
#endif
    
    contextFlg = true;

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Program_Create(GLuint init_ID, const char *init_label) {
    
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);

	if (checkID<program*>(init_ID, __func__, programvec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (checklabel<program*>(init_label, __func__, programvec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);

	program *pr = new program(init_ID, init_label);
	pr->linkFlg = false;
	programvec.push_back(pr);


	pr->handler = glCreateProgram();

	return EXIT(ERRCHK_SUCCESS);
}


ERRenum GLcommon::Program_AttachShader(GLuint programID, GLuint shaderID)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    GLuint programindex,shaderindex;
    bool conflict_flg;
    if(findID<program*>(programID, __func__, programvec, programindex) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    if(findID<shader*>(shaderID,__func__,shadervec,shaderindex) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    
    std::vector<shader*>::iterator k = (programvec[programindex]->attachedShadervec).begin();
    
    while(k != (programvec[programindex]->attachedShadervec).end() && (*k)->type != shadervec[shaderindex]->type) ++k;
    if(k != (programvec[programindex]->attachedShadervec).end()) {
        printf("WARNING (%s) : Collision of shader type detected. Detaching operation is being excuted.\n",__func__);
        
        glDetachShader(programvec[programindex]->handler, (*k)->handler);
        
        (programvec[programindex]->attachedShadervec).erase(k);
    }
    
    (programvec[programindex]->attachedShadervec).push_back(shadervec[shaderindex]);
    
    glAttachShader(programvec[programindex]->handler, shadervec[shaderindex]->handler);
    
    return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Program_LinkShader(GLuint ID){
    
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    GLuint index;
    if(findID<program*>(ID, __func__, programvec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    
    for(std::vector<shader*>::iterator i = programvec[index]->attachedShadervec.begin(); i != programvec[index]->attachedShadervec.end(); ++i)
    {
        for(std::vector<attribLocation>::iterator j = (*i)->attribLoc.begin(); j != (*i)->attribLoc.end(); ++j)
        {
            glBindAttribLocation(programvec[index]->handler, (*j).index, (*j).name);
        }
    }

	glLinkProgram(programvec[index]->handler);

	GLint link_status;

	glGetProgramiv(programvec[index]->handler, GL_LINK_STATUS, &link_status);

	if (link_status == GL_TRUE) {
		printf("INFO (%s) : Succeeded to link program.\n", __func__);
		programvec[index]->linkFlg = true;
		return EXIT(ERRCHK_SUCCESS);
	}
	else {

		printf("SUSPENDED (%s) : Failed to link program. Log detail is provided below. \n", __func__);

		GLint infologlength;
		glGetProgramiv(programvec[index]->handler, GL_INFO_LOG_LENGTH, &infologlength);

		if (infologlength > 1)
		{
			printf("-----------------------------------------------------------------\n");
			GLchar *log = (GLchar*)malloc(infologlength);
			
			glGetProgramInfoLog(programvec[index]->handler, infologlength, NULL, log);

			printf("%s\n", log);
			printf("-----------------------------------------------------------------\n");
			free(log);
		}
        
		return EXIT(ERRCHK_SUSPEND);
	}
    
    
}

ERRenum GLcommon::Program_USE(GLuint ID, std::function<void(void)> fn) {
	ERRCHECK();
	GLuint index = 0;
	if (findID<program*>(ID, __func__, programvec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	glUseProgram(programvec[index]->handler);

	fn();

	glUseProgram(0);
	if (!errchk) return EXIT(ERRCHK_SUSPEND);
	else return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Program_USE(GLuint ID) {
	ERRCHECK();
	GLuint index = 0;
	if (findID<program*>(ID, __func__, programvec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	glUseProgram(programvec[index]->handler);

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Program_Unbind() {
	glUseProgram(0);
	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Shader_Create(GLuint init_ID, const char *init_label, GLenum init_type, const char * init_path )
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
	if (checkID<shader*>(init_ID, __func__, shadervec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (checklabel<shader*>(init_label, __func__, shadervec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);

	std::ifstream file;

	file.open(init_path);
	if (file.fail()) {
		printf("SUSPENDED [%s] , (%s) : shader source not found.\n", init_label ,__func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}

	std::string str_buffer = "";
	std::string str_total = "";

	while (std::getline(file, str_buffer)) {
		str_total += str_buffer + "\n";
	}
	
	const char *src = str_total.c_str();


	shader *sh = new shader(init_ID, init_label, init_type, init_path, src);
	sh->compileFlg = false;
	shadervec.push_back(sh);

	sh->handler = glCreateShader(sh->type);


	glShaderSource(sh->handler, 1, &src, NULL);
	glCompileShader(sh->handler);

	GLint checkCompileStatus;
	glGetShaderiv(sh->handler, GL_COMPILE_STATUS, &checkCompileStatus);
	if (checkCompileStatus == GL_TRUE) {
		printf("INFO (%s) : Succeeded to compile shader.\n", __func__);
		sh->compileFlg = true;
		return EXIT(ERRCHK_SUCCESS);
	}
	else {

		printf("SUSPENDED (%s) : Failed to compile shader. Log detail is provided below. \n", __func__);

		GLint shaderInfoLogLength;
		glGetShaderiv(sh->handler, GL_INFO_LOG_LENGTH, &shaderInfoLogLength);
		if (shaderInfoLogLength > 1) {

			printf("-----------------------------------------------------------------\n");

			GLchar *shaderCompileLog = (GLchar *)malloc(shaderInfoLogLength);
			glGetShaderInfoLog(sh->handler, shaderInfoLogLength, NULL, shaderCompileLog);

			printf("%s\n", shaderCompileLog);

			printf("-----------------------------------------------------------------\n");

			free(shaderCompileLog);
		}
        
		return EXIT(ERRCHK_SUSPEND);
	}
    
    
}

ERRenum GLcommon::Shader_AddAttribLocation(GLuint ID, const char *loc_name, GLuint loc_index){
    
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    GLuint index;
    if(findID<shader*>(ID, __func__, shadervec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    attribLocation attrLoc = {loc_index,(char *)loc_name};
    shadervec[index]->attribLoc.push_back(attrLoc);
    return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Texture_Create(GLuint init_ID, const char *init_label, unsigned short width, unsigned short height) {
    
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);

	if (checkID<tex*>(init_ID, __func__, texturevec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (checklabel<tex*>(init_label, __func__, texturevec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);

	if (width == 0 || height == 0) {
		printf("SUSPENDED (%s) : Value of texture's width or height should be greater than zero.\n");
		return EXIT(ERRCHK_SUSPEND);
	}

    tex* texture = new tex(init_ID,init_label,width,height);
	texturevec.push_back(texture);

	glGenTextures(1, &(texture->handler));

    
    return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::Texture_Store(GLuint init_ID, const void *srcData, GLint internalformat, GLenum internaltype) {
    
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    GLenum textureformat = CONVERT(internalformat);
    
    if(textureformat == 0){
        printf("SUSPENDED (%s) : Illegal texture internal format.\n", __func__);
        
        return EXIT(ERRCHK_SUSPEND);
    }
    
	std::vector<tex*>::iterator itr = texturevec.begin();
	while ((*itr)->ID != init_ID) {
		++itr;
		if (itr == texturevec.end()) {
			printf("SUSPENDED (%s) : Texture not registered.\n",__func__);
            
			return EXIT(ERRCHK_SUSPEND);
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, (*itr)->handler);

	(*itr)->internalformat = internalformat;
	(*itr)->internaltype = internaltype;


	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, (*itr)->width, (*itr)->height, 0, textureformat, internaltype, srcData);

	glBindTexture(GL_TEXTURE_2D, 0);

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VAO_Create(GLuint init_ID, const char * init_label)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    if (checkID<vao*>(init_ID, __func__, vaovec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    if (checklabel<vao*>(init_label, __func__, vaovec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    
    vao *newvao = new vao(init_ID,init_label);
    if(newvao == NULL){
        printf("SUSPENDED (%s) : failed to create vao object.\n", __func__);
        
        return EXIT(ERRCHK_SUSPEND);
    }
    
    vaovec.push_back(newvao);
    
    glGenVertexArrays(1,&(newvao->handler));
    
    if(newvao->handler == 0)
    {
        printf("SUSPENDED (%s) : failed to initalize vao.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
    }
    
    return EXIT(ERRCHK_SUCCESS);
}
ERRenum GLcommon::VAO_VertexAttribArray_Register(GLuint vao_ID, GLuint vbo_ID, GLuint shader_ID, GLuint location)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
	GLuint vaoIndex, vboIndex, shaderIndex, shaderAttribLocIndex;
	if (findID<vao*>(vao_ID, __func__, vaovec, vaoIndex) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (findID<vbo*>(vbo_ID, __func__, vbovec, vboIndex) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (findID<shader*>(shader_ID, __func__, shadervec, shaderIndex) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	
	std::vector<attribLocation>::iterator i = shadervec[shaderIndex]->attribLoc.begin();
	while (i != shadervec[shaderIndex]->attribLoc.end() && (*i).index != location) ++i;
	if (i == shadervec[shaderIndex]->attribLoc.end()) {
		printf("SUSPENDED (%s) : Invalid attribute location.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}
	
	printf("NOTE (%s) : Registering location %d (%s) [vao label : %s, vbo label : %s]", __func__, location, (*i).name, vaovec[vaoIndex]->label, vbovec[vboIndex]->label);


	glBindVertexArray(vaovec[vaoIndex]->handler);

	glBindBuffer(GL_ARRAY_BUFFER, vbovec[vboIndex]->handler);

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, vbovec[vboIndex]->vertex_dim, vbovec[vboIndex]->type, vbovec[vboIndex]->normalized, vbovec[vboIndex]->stride, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
    
    return EXIT(ERRCHK_SUCCESS);
    
}

ERRenum GLcommon::VAO_USE(GLuint ID, std::function<void(void)> fn)
{
	ERRCHECK();
	GLuint index = 0;
	if (findID<vao*>(ID, __func__, vaovec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	glBindVertexArray(vaovec[index]->handler);

	fn();

	glBindVertexArray(0);
	if (!errchk) return EXIT(ERRCHK_SUSPEND);
	else return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VAO_USE(GLuint ID)
{
	ERRCHECK();
	GLuint index = 0;
	if (findID<vao*>(ID, __func__, vaovec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	glBindVertexArray(vaovec[index]->handler);

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VAO_Unbind() {
	glBindVertexArray(0);
	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VBO_Create(GLuint init_ID, const char *init_label)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
	if (checkID<vbo*>(init_ID, __func__, vbovec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
	if (checklabel<vbo*>(init_label, __func__, vbovec) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    

	vbo *newvbo = new vbo(init_ID, init_label);

	if (newvbo == NULL) {
		printf("SUSPENDED (%s) : failed to initialize vbo.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}
    
    vbovec.push_back(newvbo);

	glGenBuffers(1, &(newvbo->handler));

	if (newvbo->handler == 0) {
		printf("SUSPENDED (%s) : failed to initialize vbo.\n", __func__);
        
		return EXIT(ERRCHK_SUSPEND);
	}

	return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VBO_StoreEmpty(GLuint ID, GLsizei bytesize, GLenum usage){
    ERRCHECK();
    GLuint index = 0;
    if(findID<vbo*>(ID, __func__, vbovec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    vbovec[index]->usage = usage;
	vbovec[index]->bytesize = bytesize;
    glBindBuffer(GL_ARRAY_BUFFER, vbovec[index]->handler);
    glBufferData(GL_ARRAY_BUFFER, bytesize, 0, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return EXIT(ERRCHK_SUCCESS);
}

ERRenum GLcommon::VBO_StoreData(GLuint ID, GLuint vertex_dim, GLuint vertex_total, GLenum type, GLenum usage, GLboolean normalized, GLsizei stride, GLsizei bytesize, const void *src_data)
{
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__))return EXIT(ERRCHK_CONTEXT_ABORT);
    
    GLuint index = 0;
    
    if(findID<vbo*>(ID, __func__, vbovec, index) != ERRCHK_SUCCESS) return EXIT(ERRCHK_SUSPEND);
    
    vbovec[index]->vertex_dim = vertex_dim;
    vbovec[index]->vertex_total = vertex_total;
    vbovec[index]->type = type;
    vbovec[index]->usage = usage;
    vbovec[index]->normalized = normalized;
    vbovec[index]->stride = stride;
    vbovec[index]->src_data = src_data;
	vbovec[index]->bytesize = bytesize;
    
    glBindBuffer(GL_ARRAY_BUFFER,vbovec[index]->handler);
    
    glBufferData(GL_ARRAY_BUFFER, bytesize, src_data, usage);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	return EXIT(ERRCHK_SUCCESS);
}
inline ERRenum GLcommon::EXIT(ERRenum err){
    if(err != ERRCHK_SUCCESS) errchk = false;
    return err;
}

inline bool GLcommon::checkContextFlg(const char *funcname){
    if(contextFlg) return true;
    else{
        printf("ABORT (%s) : Make sure createWindowandMakeContext(int,int) function is called before this function.\n",funcname);
        
        return false;
    }
}

ERRenum GLcommon::Draw(std::function<void(void)> fn){
    if(!errchk) return EXIT(ERRCHK_SKIP);
    if(!checkContextFlg(__func__)) return EXIT(ERRCHK_CONTEXT_ABORT);
    
    do{
        fn();
        flush();
    }while(glfwWindowShouldClose(window) == GL_FALSE && errchk);
    
	if (!errchk) return EXIT(ERRCHK_SUSPEND);
	else return EXIT(ERRCHK_SUCCESS);
}

void GLcommon::flush()
{
	glfwSwapBuffers(window);
	glfwWaitEvents();
}

bool GLcommon::closeflg()
{
	return glfwWindowShouldClose(window) == GL_FALSE;
}

