 void BindSkiaToInProcessGL() {
   static bool host_StubGL_installed = false;
   if (!host_StubGL_installed) {
     GrGLBinding binding;
     switch (gfx::GetGLImplementation()) {
       case gfx::kGLImplementationNone:
         NOTREACHED();
         return;
       case gfx::kGLImplementationDesktopGL:
         binding = kDesktop_GrGLBinding;
         break;
       case gfx::kGLImplementationOSMesaGL:
         binding = kDesktop_GrGLBinding;
         break;
       case gfx::kGLImplementationEGLGLES2:
         binding = kES2_GrGLBinding;
         break;
        case gfx::kGLImplementationMockGL:
          NOTREACHED();
          return;
//       default:
//         NOTREACHED();
//         return;
      }
  
      static GrGLInterface host_gl_interface = {
       binding,
 
       kProbe_GrGLCapability,    
       kProbe_GrGLCapability,    
       kProbe_GrGLCapability,    
 
       StubGLActiveTexture,
       StubGLAttachShader,
       StubGLBindAttribLocation,
       StubGLBindBuffer,
       StubGLBindTexture,
       StubGLBlendColor,
       StubGLBlendFunc,
       StubGLBufferData,
       StubGLBufferSubData,
       StubGLClear,
       StubGLClearColor,
       StubGLClearStencil,
       NULL,   
       NULL,   
       StubGLColorMask,
       NULL,   
       StubGLCompileShader,
       StubGLCompressedTexImage2D,
       StubGLCreateProgram,
       StubGLCreateShader,
       StubGLCullFace,
       StubGLDeleteBuffers,
       StubGLDeleteProgram,
       StubGLDeleteShader,
       StubGLDeleteTextures,
       StubGLDepthMask,
       StubGLDisable,
       NULL,   
       StubGLDisableVertexAttribArray,
       StubGLDrawArrays,
       StubGLDrawElements,
       StubGLEnable,
       NULL,   
       StubGLEnableVertexAttribArray,
       StubGLFrontFace,
       StubGLGenBuffers,
       StubGLGenTextures,
       StubGLGetBufferParameteriv,
       StubGLGetError,
       StubGLGetIntegerv,
       StubGLGetProgramInfoLog,
       StubGLGetProgramiv,
       StubGLGetShaderInfoLog,
       StubGLGetShaderiv,
       StubGLGetString,
       StubGLGetUniformLocation,
       StubGLLineWidth,
       StubGLLinkProgram,
       NULL,   
       NULL,   
       StubGLPixelStorei,
       NULL,   
       StubGLReadPixels,
       StubGLScissor,
       NULL,   
       StubGLShaderSource,
       StubGLStencilFunc,
       StubGLStencilFuncSeparate,
       StubGLStencilMask,
       StubGLStencilMaskSeparate,
       StubGLStencilOp,
       StubGLStencilOpSeparate,
       NULL,   
       NULL,   
       StubGLTexImage2D,
       StubGLTexParameteri,
       StubGLTexSubImage2D,
       StubGLUniform1f,
       StubGLUniform1i,
       StubGLUniform1fv,
       StubGLUniform1iv,
       StubGLUniform2f,
       StubGLUniform2i,
       StubGLUniform2fv,
       StubGLUniform2iv,
       StubGLUniform3f,
       StubGLUniform3i,
       StubGLUniform3fv,
       StubGLUniform3iv,
       StubGLUniform4f,
       StubGLUniform4i,
       StubGLUniform4fv,
       StubGLUniform4iv,
       StubGLUniformMatrix2fv,
       StubGLUniformMatrix3fv,
       StubGLUniformMatrix4fv,
       StubGLUseProgram,
       StubGLVertexAttrib4fv,
       StubGLVertexAttribPointer,
       NULL,   
       StubGLViewport,
       StubGLBindFramebuffer,
       StubGLBindRenderbuffer,
       StubGLCheckFramebufferStatus,
       StubGLDeleteFramebuffers,
       StubGLDeleteRenderbuffers,
       StubGLFramebufferRenderbuffer,
       StubGLFramebufferTexture2D,
       StubGLGenFramebuffers,
       StubGLGenRenderbuffers,
       StubGLRenderBufferStorage,
       StubGLRenderbufferStorageMultisample,
       StubGLBlitFramebuffer,
       NULL,   
       StubGLMapBuffer,
       StubGLUnmapBuffer,
       NULL,   
       GrGLInterface::kStaticInitEndGuard,
     };
     GrGLSetGLInterface(&host_gl_interface);
     host_StubGL_installed = true;
   }
 }