#ifndef SHADERUTILS_HPP
#define SHADERUTILS_HPP

#include "../Constants.hpp"

/* Maximum size per shader file (with include). 64 Ko */
#define MAX_SHADER_BUFFERSIZE  (128u*1024u)

class ShaderUtils
{
public:
    ShaderUtils();

    void InitGL();
    GLuint CreateRenderProgram(char const* vsfile, const char *gsfile, char const* fsfile, char *src_buffer);
    GLuint CreateRenderProgram(char const* vsfile, char const* fsfile, char *src_buffer);
    GLuint CreateComputeProgram(char const* program_name, char *src_buffer);
    void CheckShaderStatus(GLuint shader, char const* name);
    bool CheckProgramStatus(GLuint program, char const* name);
    void CheckGLError(char const* file, int const line, char const* errMsg, bool bExitOnFail);
    bool IsBufferBound(GLenum pname, GLuint buffer);
    GLuint GetAttribLocation(GLuint const pgm, char const *name);
    GLuint GetUniformLocation(GLuint const pgm, char const *name);
    GLuint GetUInteger(GLenum const pname);

};

#endif // SHADERUTILS_HPP
