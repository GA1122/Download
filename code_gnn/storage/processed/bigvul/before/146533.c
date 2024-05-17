void WebGLRenderingContextBase::renderbufferStorage(GLenum target,
                                                    GLenum internalformat,
                                                    GLsizei width,
                                                    GLsizei height) {
  const char* function_name = "renderbufferStorage";
  if (isContextLost())
    return;
  if (target != GL_RENDERBUFFER) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
    return;
  }
  if (!renderbuffer_binding_ || !renderbuffer_binding_->Object()) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "no bound renderbuffer");
    return;
  }
  if (!ValidateSize(function_name, width, height))
    return;
  RenderbufferStorageImpl(target, 0, internalformat, width, height,
                          function_name);
  ApplyStencilTest();
}
