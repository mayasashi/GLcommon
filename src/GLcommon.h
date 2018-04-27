//
//  GLcommon.h
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/10.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#ifndef GLcommon_h
#define GLcommon_h

#include "GLcommon_includes.h"


typedef enum {
    ERRCHK_SUCCESS,
    ERRCHK_SUSPEND,
    ERRCHK_CONTEXT_ABORT,
    ERRCHK_SKIP,
    ERRCHK_UNKNOWN
}ERRenum;

typedef enum {
    ATTRIB_LOCATION,
    UNIFORM_LOCATION
}LocationEnum;

struct GLcontext {
    GLuint GL_version_major;
    GLuint GL_version_minor;
    bool ForwardCompatFlg;
    bool CoreProfileFlg;
};

struct windowInfo {
    GLuint width;
    GLuint height;
    char *title;
    GLcontext context;
};

struct attribLocation {
    GLuint index;
    char *name;
};

using uniformLocation = attribLocation;

struct shader {
    const GLuint ID;
    const char *label;
    const GLenum type;
    const char *path;
    const char *src;
    GLuint handler = 0;
    std::vector<attribLocation> attribLoc;
    std::vector<uniformLocation> uniformLoc;
    bool compileFlg;
    
    shader(const GLuint init_ID, const char * init_label, const GLenum init_type, const char * init_path, const char * init_src) :
    ID(init_ID),
    label(init_label),
    type(init_type),
    path(init_path),
    src(init_src)
    {}
};

struct program {
    const GLuint ID;
    const char *label;
    GLuint handler = 0;
    std::vector<shader*> attachedShadervec;
    bool linkFlg;
    
    program(const GLuint init_ID, const char *init_label) :
    ID(init_ID),
    label(init_label)
    {}
};

struct tex {
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
    
    tex(GLuint init_ID, const char *init_label, GLuint init_width, GLuint init_height) :
    ID(init_ID),
    label(init_label),
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
	GLsizei bytesize;
    GLuint vertex_dim;
    GLuint vertex_total;
    GLenum type;
    GLenum usage;
    GLboolean normalized;
    GLsizei stride;
    
    vbo(GLuint init_ID, const char *init_label) :
    ID(init_ID),
    label(init_label)
    {
        src_data = NULL;
        vertex_dim = 0;
        vertex_total = 0;
        normalized = GL_FALSE;
    }
    
};

struct vertexAttribArray
{
    vbo* vbo_ptr;
    shader* shader_ptr;
    GLuint location;
};

struct vao
{
    const GLuint ID;
    const char *label;
    GLuint handler = 0;
    std::vector<vertexAttribArray> vertexAttribArray_container;
    vao(GLuint init_ID, const char *init_label) :
    ID(init_ID),
    label(init_label)
    {}
};



class GLcommon {
    
public:
    GLcommon();
    ~GLcommon();
    
    ERRenum createWindowandMakeContext(unsigned short width, unsigned short height);
    
    ERRenum Texture_Create(GLuint init_ID, const char *init_label, unsigned short width, unsigned short height);
    ERRenum Texture_Store(GLuint init_ID, const void *srcData, GLint internalFormat, GLenum internaltype);
    ERRenum Texture_SetOption(GLuint ID, GLuint MinFlg, GLuint MagFlg);   //No implementation
    ERRenum Texture_Register(GLuint textureID, GLuint shaderID, GLuint index);   //No implementation
    
    ERRenum Shader_Create(GLuint init_ID, const char *init_label, GLenum init_type, const char * init_path);
    ERRenum Shader_AddAttribLocation(GLuint ID, const char *loc_name, GLuint loc_index);
    
    ERRenum Program_Create(GLuint init_ID, const char *init_label);
    ERRenum Program_AttachShader(GLuint programID, GLuint shaderID);
    ERRenum Program_LinkShader(GLuint ID);
    ERRenum Program_USE(GLuint ID,std::function<void(void)> fn);
	ERRenum Program_USE(GLuint ID);
	ERRenum Program_Unbind();
    
    ERRenum VAO_Create(GLuint init_ID, const char * init_label);
    ERRenum VAO_VertexAttribArray_Register(GLuint vao_ID, GLuint vbo_ID, GLuint shader_ID, GLuint location);
    ERRenum VAO_USE(GLuint ID, std::function<void(void)> fn);
	ERRenum VAO_USE(GLuint ID);
	ERRenum VAO_Unbind();
    
    ERRenum VBO_Create(GLuint init_ID, const char *init_label);
    ERRenum VBO_StoreEmpty(GLuint ID, GLsizei bytesize, GLenum usage);
    ERRenum VBO_StoreData(GLuint ID, GLuint vertex_dim, GLuint vertex_total, GLenum type, GLenum usage, GLboolean normalized, GLsizei stride, GLsizei bytesize, const void *src_data);
    ERRenum VBO_SwapData(GLuint ID, const void *data);  //No implementation
    
    
    ERRenum Draw(std::function<void(void)> fn);
    void flush();
    bool closeflg();
    
private:
    template <typename T> ERRenum checkID(GLuint ID, const char *func_name, const std::vector<T>& vec);
    template <typename T> ERRenum findID(GLuint ID, const char *func_name, const std::vector<T>& vec, GLuint &index);
    template <typename T> ERRenum checklabel(const char *label, const char *func_name, const std::vector<T>& vec);
    ERRenum EXIT(ERRenum err);
    bool checkContextFlg(const char *funcname);
    GLFWwindow *window;
    std::vector<tex*> texturevec;
    std::vector<shader*> shadervec;
    std::vector<program*> programvec;
    std::vector<vbo*> vbovec;
    std::vector<vao*> vaovec;
    bool contextFlg;
    bool errchk;
};


#endif /* GLcommon_h */
