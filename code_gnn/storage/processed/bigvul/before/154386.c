bool GLES2DecoderImpl::ValidateRenderbufferStorageMultisampleAMD(
    GLsizei samples,
    GLsizei storageSamples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  if (samples > renderbuffer_manager()->max_samples()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glRenderbufferStorageMultisample",
                       "samples too large");
    return false;
  }

  if (width > renderbuffer_manager()->max_renderbuffer_size() ||
      height > renderbuffer_manager()->max_renderbuffer_size()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glRenderbufferStorageMultisample",
                       "dimensions too large");
    return false;
  }

  uint32_t estimated_size = 0;
  if (!renderbuffer_manager()->ComputeEstimatedRenderbufferSize(
          width, height, samples, internalformat, &estimated_size)) {
    LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY, "glRenderbufferStorageMultisample",
                       "dimensions too large");
    return false;
  }

  return true;
}
