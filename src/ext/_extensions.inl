// This file was generated by a script @ 2021/10/23 14:39:48

PFNGLACTIVETEXTUREPROC pfnActiveTexture;
PFNGLATTACHSHADERPROC pfnAttachShader;
PFNGLBEGINQUERYPROC pfnBeginQuery;
PFNGLBINDBUFFERPROC pfnBindBuffer;
PFNGLBINDBUFFERBASEPROC pfnBindBufferBase;
PFNGLBINDBUFFERRANGEPROC pfnBindBufferRange;
PFNGLBINDBUFFERSBASEPROC pfnBindBuffersBase;
PFNGLBINDSAMPLERPROC pfnBindSampler;
PFNGLBINDVERTEXARRAYPROC pfnBindVertexArray;
PFNGLBINDVERTEXBUFFERPROC pfnBindVertexBuffer;
PFNGLBLENDEQUATIONPROC pfnBlendEquation;
PFNGLBLENDEQUATIONSEPARATEPROC pfnBlendEquationSeparate;
PFNGLBLENDFUNCSEPARATEPROC pfnBlendFuncSeparate;
PFNGLBUFFERDATAPROC pfnBufferData;
PFNGLBUFFERSTORAGEPROC pfnBufferStorage;
PFNGLBUFFERSUBDATAPROC pfnBufferSubData;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC pfnClearNamedBufferSubData;
PFNGLCOMPILESHADERPROC pfnCompileShader;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC pfnCopyNamedBufferSubData;
PFNGLCREATEPROGRAMPROC pfnCreateProgram;
PFNGLCREATEQUERIESPROC pfnCreateQueries;
PFNGLCREATESHADERPROC pfnCreateShader;
PFNGLCREATESHADERPROGRAMVPROC pfnCreateShaderProgramv;
PFNGLDELETEBUFFERSPROC pfnDeleteBuffers;
PFNGLDELETEPROGRAMPROC pfnDeleteProgram;
PFNGLDELETEQUERIESPROC pfnDeleteQueries;
PFNGLDELETESHADERPROC pfnDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC pfnDeleteVertexArrays;
PFNGLDETACHSHADERPROC pfnDetachShader;
PFNGLDISPATCHCOMPUTEPROC pfnDispatchCompute;
PFNGLDISPATCHCOMPUTEINDIRECTPROC pfnDispatchComputeIndirect;
PFNGLDRAWARRAYSINDIRECTPROC pfnDrawArraysIndirect;
PFNGLENABLEVERTEXATTRIBARRAYPROC pfnEnableVertexAttribArray;
PFNGLENDQUERYPROC pfnEndQuery;
PFNGLGENBUFFERSPROC pfnGenBuffers;
PFNGLGENVERTEXARRAYSPROC pfnGenVertexArrays;
PFNGLGENERATEMIPMAPPROC pfnGenerateMipmap;
PFNGLGETATTRIBLOCATIONPROC pfnGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC pfnGetProgramInfoLog;
PFNGLGETPROGRAMRESOURCEINDEXPROC pfnGetProgramResourceIndex;
PFNGLGETPROGRAMIVPROC pfnGetProgramiv;
PFNGLGETQUERYOBJECTUIVPROC pfnGetQueryObjectuiv;
PFNGLGETSHADERINFOLOGPROC pfnGetShaderInfoLog;
PFNGLGETSHADERIVPROC pfnGetShaderiv;
PFNGLGETUNIFORMBLOCKINDEXPROC pfnGetUniformBlockIndex;
PFNGLGETUNIFORMLOCATIONPROC pfnGetUniformLocation;
PFNGLISPROGRAMPROC pfnIsProgram;
PFNGLLINKPROGRAMPROC pfnLinkProgram;
PFNGLMAPBUFFERPROC pfnMapBuffer;
PFNGLMAPBUFFERRANGEPROC pfnMapBufferRange;
PFNGLMAPNAMEDBUFFERRANGEPROC pfnMapNamedBufferRange;
PFNGLMEMORYBARRIERPROC pfnMemoryBarrier;
PFNGLNAMEDBUFFERSUBDATAPROC pfnNamedBufferSubData;
PFNGLPROGRAMUNIFORM1IPROC pfnProgramUniform1i;
PFNGLSHADERSOURCEPROC pfnShaderSource;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC pfnShaderStorageBlockBinding;
PFNGLTEXSTORAGE2DPROC pfnTexStorage2D;
PFNGLTEXSTORAGE3DPROC pfnTexStorage3D;
PFNGLTEXSUBIMAGE3DPROC pfnTexSubImage3D;
PFNGLUNIFORM1FPROC pfnUniform1f;
PFNGLUNIFORM1IPROC pfnUniform1i;
PFNGLUNIFORM1UIPROC pfnUniform1ui;
PFNGLUNIFORM3FPROC pfnUniform3f;
PFNGLUNIFORM3FVPROC pfnUniform3fv;
PFNGLUNIFORM4FPROC pfnUniform4f;
PFNGLUNIFORM4FVPROC pfnUniform4fv;
PFNGLUNIFORMMATRIX3FVPROC pfnUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC pfnUniformMatrix4fv;
PFNGLUNMAPBUFFERPROC pfnUnmapBuffer;
PFNGLUSEPROGRAMPROC pfnUseProgram;
PFNGLVALIDATEPROGRAMPROC pfnValidateProgram;
PFNGLVERTEXATTRIBBINDINGPROC pfnVertexAttribBinding;
PFNGLVERTEXATTRIBFORMATPROC pfnVertexAttribFormat;
PFNGLVERTEXATTRIBIFORMATPROC pfnVertexAttribIFormat;
PFNGLVERTEXATTRIBPOINTERPROC pfnVertexAttribPointer;

static
void LoadExtensionFuncPtrs() {
  pfnActiveTexture = (PFNGLACTIVETEXTUREPROC)
      getAddress("glActiveTexture");

  pfnAttachShader = (PFNGLATTACHSHADERPROC)
      getAddress("glAttachShader");

  pfnBeginQuery = (PFNGLBEGINQUERYPROC)
      getAddress("glBeginQuery");

  pfnBindBuffer = (PFNGLBINDBUFFERPROC)
      getAddress("glBindBuffer");

  pfnBindBufferBase = (PFNGLBINDBUFFERBASEPROC)
      getAddress("glBindBufferBase");

  pfnBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)
      getAddress("glBindBufferRange");

  pfnBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)
      getAddress("glBindBuffersBase");

  pfnBindSampler = (PFNGLBINDSAMPLERPROC)
      getAddress("glBindSampler");

  pfnBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)
      getAddress("glBindVertexArray");

  pfnBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)
      getAddress("glBindVertexBuffer");

  pfnBlendEquation = (PFNGLBLENDEQUATIONPROC)
      getAddress("glBlendEquation");

  pfnBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)
      getAddress("glBlendEquationSeparate");

  pfnBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)
      getAddress("glBlendFuncSeparate");

  pfnBufferData = (PFNGLBUFFERDATAPROC)
      getAddress("glBufferData");

  pfnBufferStorage = (PFNGLBUFFERSTORAGEPROC)
      getAddress("glBufferStorage");

  pfnBufferSubData = (PFNGLBUFFERSUBDATAPROC)
      getAddress("glBufferSubData");

  pfnClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)
      getAddress("glClearNamedBufferSubData");

  pfnCompileShader = (PFNGLCOMPILESHADERPROC)
      getAddress("glCompileShader");

  pfnCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)
      getAddress("glCopyNamedBufferSubData");

  pfnCreateProgram = (PFNGLCREATEPROGRAMPROC)
      getAddress("glCreateProgram");

  pfnCreateQueries = (PFNGLCREATEQUERIESPROC)
      getAddress("glCreateQueries");

  pfnCreateShader = (PFNGLCREATESHADERPROC)
      getAddress("glCreateShader");

  pfnCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)
      getAddress("glCreateShaderProgramv");

  pfnDeleteBuffers = (PFNGLDELETEBUFFERSPROC)
      getAddress("glDeleteBuffers");

  pfnDeleteProgram = (PFNGLDELETEPROGRAMPROC)
      getAddress("glDeleteProgram");

  pfnDeleteQueries = (PFNGLDELETEQUERIESPROC)
      getAddress("glDeleteQueries");

  pfnDeleteShader = (PFNGLDELETESHADERPROC)
      getAddress("glDeleteShader");

  pfnDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)
      getAddress("glDeleteVertexArrays");

  pfnDetachShader = (PFNGLDETACHSHADERPROC)
      getAddress("glDetachShader");

  pfnDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)
      getAddress("glDispatchCompute");

  pfnDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)
      getAddress("glDispatchComputeIndirect");

  pfnDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)
      getAddress("glDrawArraysIndirect");

  pfnEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
      getAddress("glEnableVertexAttribArray");

  pfnEndQuery = (PFNGLENDQUERYPROC)
      getAddress("glEndQuery");

  pfnGenBuffers = (PFNGLGENBUFFERSPROC)
      getAddress("glGenBuffers");

  pfnGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)
      getAddress("glGenVertexArrays");

  pfnGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)
      getAddress("glGenerateMipmap");

  pfnGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)
      getAddress("glGetAttribLocation");

  pfnGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
      getAddress("glGetProgramInfoLog");

  pfnGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)
      getAddress("glGetProgramResourceIndex");

  pfnGetProgramiv = (PFNGLGETPROGRAMIVPROC)
      getAddress("glGetProgramiv");

  pfnGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)
      getAddress("glGetQueryObjectuiv");

  pfnGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
      getAddress("glGetShaderInfoLog");

  pfnGetShaderiv = (PFNGLGETSHADERIVPROC)
      getAddress("glGetShaderiv");

  pfnGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)
      getAddress("glGetUniformBlockIndex");

  pfnGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)
      getAddress("glGetUniformLocation");

  pfnIsProgram = (PFNGLISPROGRAMPROC)
      getAddress("glIsProgram");

  pfnLinkProgram = (PFNGLLINKPROGRAMPROC)
      getAddress("glLinkProgram");

  pfnMapBuffer = (PFNGLMAPBUFFERPROC)
      getAddress("glMapBuffer");

  pfnMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)
      getAddress("glMapBufferRange");

  pfnMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC)
      getAddress("glMapNamedBufferRange");

  pfnMemoryBarrier = (PFNGLMEMORYBARRIERPROC)
      getAddress("glMemoryBarrier");

  pfnNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)
      getAddress("glNamedBufferSubData");

  pfnProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)
      getAddress("glProgramUniform1i");

  pfnShaderSource = (PFNGLSHADERSOURCEPROC)
      getAddress("glShaderSource");

  pfnShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)
      getAddress("glShaderStorageBlockBinding");

  pfnTexStorage2D = (PFNGLTEXSTORAGE2DPROC)
      getAddress("glTexStorage2D");

  pfnTexStorage3D = (PFNGLTEXSTORAGE3DPROC)
      getAddress("glTexStorage3D");

  pfnTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)
      getAddress("glTexSubImage3D");

  pfnUniform1f = (PFNGLUNIFORM1FPROC)
      getAddress("glUniform1f");

  pfnUniform1i = (PFNGLUNIFORM1IPROC)
      getAddress("glUniform1i");

  pfnUniform1ui = (PFNGLUNIFORM1UIPROC)
      getAddress("glUniform1ui");

  pfnUniform3f = (PFNGLUNIFORM3FPROC)
      getAddress("glUniform3f");

  pfnUniform3fv = (PFNGLUNIFORM3FVPROC)
      getAddress("glUniform3fv");

  pfnUniform4f = (PFNGLUNIFORM4FPROC)
      getAddress("glUniform4f");

  pfnUniform4fv = (PFNGLUNIFORM4FVPROC)
      getAddress("glUniform4fv");

  pfnUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)
      getAddress("glUniformMatrix3fv");

  pfnUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)
      getAddress("glUniformMatrix4fv");

  pfnUnmapBuffer = (PFNGLUNMAPBUFFERPROC)
      getAddress("glUnmapBuffer");

  pfnUseProgram = (PFNGLUSEPROGRAMPROC)
      getAddress("glUseProgram");

  pfnValidateProgram = (PFNGLVALIDATEPROGRAMPROC)
      getAddress("glValidateProgram");

  pfnVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)
      getAddress("glVertexAttribBinding");

  pfnVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)
      getAddress("glVertexAttribFormat");

  pfnVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)
      getAddress("glVertexAttribIFormat");

  pfnVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)
      getAddress("glVertexAttribPointer");

}

