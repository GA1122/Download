void WebGL2RenderingContextBase::RenderbufferStorageHelper(
    GLenum target,
    GLsizei samples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    const char* function_name) {
  if (!samples) {
    ContextGL()->RenderbufferStorage(target, internalformat, width, height);
  } else {
    GLint max_number_of_samples = 0;
    ContextGL()->GetInternalformativ(target, internalformat, GL_SAMPLES, 1,
                                     &max_number_of_samples);
    if (samples > max_number_of_samples) {
      SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                        "samples out of range");
      return;
    }
    ContextGL()->RenderbufferStorageMultisampleCHROMIUM(
        target, samples, internalformat, width, height);
  }
}
