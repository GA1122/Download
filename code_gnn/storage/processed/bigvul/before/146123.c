void WebGL2RenderingContextBase::renderbufferStorageMultisample(
    GLenum target,
    GLsizei samples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  const char* function_name = "renderbufferStorageMultisample";
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
  if (!ValidateSize("renderbufferStorage", width, height))
    return;
  if (samples < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "samples < 0");
    return;
  }
  RenderbufferStorageImpl(target, samples, internalformat, width, height,
                          function_name);
  ApplyStencilTest();
}
