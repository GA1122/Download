void WebGL2RenderingContextBase::RenderbufferStorageImpl(
    GLenum target,
    GLsizei samples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    const char* function_name) {
  switch (internalformat) {
    case GL_R8UI:
    case GL_R8I:
    case GL_R16UI:
    case GL_R16I:
    case GL_R32UI:
    case GL_R32I:
    case GL_RG8UI:
    case GL_RG8I:
    case GL_RG16UI:
    case GL_RG16I:
    case GL_RG32UI:
    case GL_RG32I:
    case GL_RGBA8UI:
    case GL_RGBA8I:
    case GL_RGB10_A2UI:
    case GL_RGBA16UI:
    case GL_RGBA16I:
    case GL_RGBA32UI:
    case GL_RGBA32I:
      if (samples > 0) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "for integer formats, samples > 0");
        return;
      }
    case GL_R8:
    case GL_RG8:
    case GL_RGB8:
    case GL_RGB565:
    case GL_RGBA8:
    case GL_SRGB8_ALPHA8:
    case GL_RGB5_A1:
    case GL_RGBA4:
    case GL_RGB10_A2:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
    case GL_STENCIL_INDEX8:
      RenderbufferStorageHelper(target, samples, internalformat, width, height,
                                function_name);
      break;
    case GL_DEPTH_STENCIL:
      if (samples > 0) {
        SynthesizeGLError(GL_INVALID_ENUM, function_name,
                          "invalid internalformat");
        return;
      }
      RenderbufferStorageHelper(target, 0, GL_DEPTH24_STENCIL8, width, height,
                                function_name);
      break;
    case GL_R16F:
    case GL_RG16F:
    case GL_RGBA16F:
    case GL_R32F:
    case GL_RG32F:
    case GL_RGBA32F:
    case GL_R11F_G11F_B10F:
      if (!ExtensionEnabled(kEXTColorBufferFloatName)) {
        SynthesizeGLError(GL_INVALID_ENUM, function_name,
                          "EXT_color_buffer_float not enabled");
        return;
      }
      RenderbufferStorageHelper(target, samples, internalformat, width, height,
                                function_name);
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name,
                        "invalid internalformat");
      return;
  }
  renderbuffer_binding_->SetInternalFormat(internalformat);
  renderbuffer_binding_->SetSize(width, height);
}
