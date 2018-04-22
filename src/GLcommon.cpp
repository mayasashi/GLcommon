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
    GLuint index;
    char *name;
};

using uniformLocation = attribLocation;

struct shader{
    const GLuint ID;
    const char *label;
    const GLenum type;
	const char *path;
    const char *src;
    GLuint handler = 0;
    std::vector<attribLocation> attribLocation;
    std::vector<uniformLocation> uniformLocation;
    bool compileFlg;
    
	shader(const GLuint init_ID, const char * init_label, const GLenum init_type, const char * init_path, const char * init_src) :
		ID(init_ID),
		label(init_label),
		type(init_type),
		path(init_path),
		src(init_src)
    {}
};

struct program{
    const GLuint ID;
    const char *label;
    GLuint handler = 0;
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
    GLuint handler = 0;
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

struct vbo
{
	const GLuint ID;
	const char *label;
	GLuint handler = 0;
	const void *src_data;
	GLuint vertex_dim;
	GLuint vertex_total;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;

	vbo(GLuint init_ID, const char *init_label) : 
		ID(init_ID),
		label(init_label)
	{}

};

struct vertexAttribArray
{
	GLuint vbo_ID;
	GLuint program_ID;

};

class GLcommon {
    
public:
    GLcommon();
    ~GLcommon();

    void createWindow(int width,int height);

	ERRenum Texture_Create(GLuint init_ID, const char *init_label, int width, int height);
	ERRenum Texture_Store(GLuint init_ID, const void *srcData, GLint internalFormat, GLenum internaltype);

    ERRenum Shader_Create(GLuint init_ID, const char *init_label, GLenum init_type, const char * init_path);
    ERRenum Program_Create(GLuint init_ID, const char *init_label);
	ERRenum Program_AttachShader(GLuint programID, GLuint shaderID, GLenum shaderType);
	ERRenum Program_LinkShader(GLuint programID);

	ERRenum Shader_AssociateAttribLocation(GLuint shaderID, const std::vector<attribLocation> &vec);
	ERRenum Shader_AssociateUniformLocation(GLuint shaderID, const std::vector<uniformLocation> &vec);

    ERRenum VAO_Create();

    ERRenum VBO_Create(GLuint init_ID, const char *init_label);
	ERRenum VBO_StoreEmpty(GLuint ID, GLsizei size);
	ERRenum VBO_StoreData(GLuint ID, GLuint vertex_dim, GLuint vertex_total, GLenum type, GLboolean normalized, GLsizei stride, const void *src_data);

    void flush();
    
private:
	template <typename T> ERRenum checkID(GLuint ID, const char *func_name, const std::vector<T>& vec);
	template <typename T> ERRenum findID(GLuint ID, const std::vector<T>& vec, GLuint &index);
	template <typename T> ERRenum checklabel(const char *label, const char *func_name, const std::vector<T>& vec);
    GLFWwindow *window;
    std::vector<tex*> texturevec;
    std::vector<shader*> shadervec;
    std::vector<program*> programvec;
	std::vector<vbo*> vbovec;
    
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
ERRenum GLcommon::checkID(GLuint ID, const char *func_name, const std::vector<T>& vec) {
	bool conflict_flg = false;
	for (typename std::vector<T>::const_iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if ((*itr)->ID == ID) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting ID specified.\n",func_name);
		return ERRCHK_SUSPEND;
	}
	return ERRCHK_SUCCESS;
}

template <typename T> ERRenum GLcommon::findID(GLuint ID, const std::vector<T>& vec, GLuint &index)
{
	typename std::vector<T>::const_iterator itr = vec.begin();
	while()
	return ERRCHK_SUCCESS;
}

template <typename T> 
ERRenum GLcommon::checklabel(const char *label, const char *func_name, const std::vector<T>& vec) {

	if (strcmp(label, "") == 0 || label == nullptr) {
		printf("SUSPENDED (%s) : label is empty.\n",func_name);
		return ERRCHK_SUSPEND;
	}
	bool conflict_flg = false;
	for (typename std::vector<T>::const_iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		if (strcmp((*itr)->label, label) == 0) conflict_flg = true;
	}
	if (conflict_flg) {
		printf("SUSPENDED (%s): excisting label specified.\n", func_name);
		return ERRCHK_SUSPEND;
	}

	return ERRCHK_SUCCESS;
}

ERRenum GLcommon::Program_Create(GLuint init_ID, const char *init_label) {

	if (checkID<program*>(init_ID, __func__, programvec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<program*>(init_label, __func__, programvec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

	program *pr = new program(init_ID, init_label);
	programvec.push_back(pr);

	pr->handler = glCreateProgram();

	return ERRCHK_SUCCESS;
}

ERRenum GLcommon::Shader_Create(GLuint init_ID, const char *init_label, GLenum init_type, const char * init_path )
{
	if (checkID<shader*>(init_ID, __func__, shadervec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<shader*>(init_label, __func__, shadervec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

	std::ifstream file;

	file.open(init_path);
	if (file.fail()) {
		printf("SUSPENDED [%s] , (%s) : shader source not found.\n", init_label ,__func__);
		return ERRCHK_SUSPEND;
	}

	std::string str_buffer = "";
	std::string str_total = "";

	while (std::getline(file, str_buffer)) {
		str_total += str_buffer + "\n";
	}
	
	const char *src = str_total.c_str();


	shader *sh = new shader(init_ID, init_label, init_type, init_path, src);
	shadervec.push_back(sh);

	sh->handler = glCreateShader(sh->type);


	glShaderSource(sh->handler, 1, &src, NULL);
	glCompileShader(sh->handler);

	GLint checkCompileStatus;
	GLint shaderInfoLogLength;
	glGetShaderiv(sh->handler, GL_COMPILE_STATUS, &checkCompileStatus);
	glGetShaderiv(sh->handler, GL_INFO_LOG_LENGTH, &shaderInfoLogLength);
	if (shaderInfoLogLength > 1) {
		GLchar *shaderCompileLog = (GLchar *)malloc(shaderInfoLogLength);
		glGetShaderInfoLog(sh->handler, shaderInfoLogLength, NULL, shaderCompileLog);
		free(shaderCompileLog);
	}
    
    return ERRCHK_SUCCESS;
}

ERRenum GLcommon::Texture_Create(GLuint init_ID, const char *init_label, int width, int height) {

	if (checkID<tex*>(init_ID, __func__, texturevec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<tex*>(init_label, __func__, texturevec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

    tex* texture = new tex(init_ID,init_label,width,height);
	texturevec.push_back(texture);

	glGenTextures(1, &(texture->handler));

    
    return ERRCHK_SUCCESS;
}

ERRenum GLcommon::Texture_Store(GLuint init_ID, const void *srcData, GLint internalformat, GLenum internaltype) {
    
    GLenum textureformat = CONVERT(internalformat);
    
    if(textureformat == 0){
        printf("SUSPENDED (%s) : Illegal texture internal format.\n", __func__);
        return ERRCHK_SUSPEND;
    }
    
	std::vector<tex*>::iterator itr = texturevec.begin();
	while ((*itr)->ID != init_ID) {
		++itr;
		if (itr == texturevec.end()) {
			printf("SUSPENDED (%s) : Texture not registered.\n",__func__);
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

ERRenum GLcommon::VBO_Create(GLuint init_ID, const char *init_label)
{
	if (checkID<vbo*>(init_ID, __func__, vbovec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;
	if (checklabel<vbo*>(init_label, __func__, vbovec) != ERRCHK_SUCCESS) return ERRCHK_SUSPEND;

	vbo *newvbo = new vbo(init_ID, init_label);

	if (newvbo == NULL) {
		printf("SUSPENDED (%s) : failed to initialize vbo.\n", __func__);
		return ERRCHK_SUSPEND;
	}

	glGenBuffers(1, &(newvbo->handler));

	if (newvbo->handler == 0) {
		printf("SUSPENDED (%s) : failed to initialize vbo.\n", __func__);
		return ERRCHK_SUSPEND;
	}

	return ERRCHK_SUCCESS;
}

ERRenum GLcommon::VBO_StoreData(GLuint ID, GLuint vertex_dim, GLuint vertex_total, GLenum type, GLboolean normalized, GLsizei stride, const void *src_data)
{

	return ERRCHK_SUCCESS;
}

