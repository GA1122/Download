void WebGLRenderingContextBase::RenderbufferStorageImpl(
    GLenum target,
    GLsizei samples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    const char* function_name) {
  DCHECK(!samples);              
  DCHECK(!IsWebGL2OrHigher());   
  switch (internalformat) {
    case GL_DEPTH_COMPONENT16:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_RGB565:
    case GL_STENCIL_INDEX8:
      ContextGL()->RenderbufferStorage(target, internalformat, width, height);
      renderbuffer_binding_->SetInternalFormat(internalformat);
      renderbuffer_binding_->SetSize(width, height);
      break;
    case GL_SRGB8_ALPHA8_EXT:
      if (!ExtensionEnabled(kEXTsRGBName)) {
        SynthesizeGLError(GL_INVALID_ENUM, function_name, "sRGB not enabled");
        break;
      }
      ContextGL()->RenderbufferStorage(target, internalformat, width, height);
      renderbuffer_binding_->SetInternalFormat(internalformat);
      renderbuffer_binding_->SetSize(width, height);
      break;
    case GL_DEPTH_STENCIL_OES:
      DCHECK(IsDepthStencilSupported());
      ContextGL()->RenderbufferStorage(target, GL_DEPTH24_STENCIL8_OES, width,
                                       height);
      renderbuffer_binding_->SetSize(width, height);
      renderbuffer_binding_->SetInternalFormat(internalformat);
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name,
                        "invalid internalformat");
      break;
  }
}
