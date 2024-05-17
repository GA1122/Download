void GLES2DecoderImpl::DoRenderbufferStorageMultisampleCHROMIUM(
    GLenum target, GLsizei samples, GLenum internalformat,
    GLsizei width, GLsizei height) {
  Renderbuffer* renderbuffer = GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glRenderbufferStorageMultisampleCHROMIUM",
                       "no renderbuffer bound");
    return;
  }

  if (!ValidateRenderbufferStorageMultisample(
      samples, internalformat, width, height)) {
    return;
  }

  GLenum impl_format =
      renderbuffer_manager()->InternalRenderbufferFormatToImplFormat(
          internalformat);
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(
      "glRenderbufferStorageMultisampleCHROMIUM");
  RenderbufferStorageMultisampleWithWorkaround(target, samples, impl_format,
                                               width, height, kDoNotForce);
  GLenum error =
      LOCAL_PEEK_GL_ERROR("glRenderbufferStorageMultisampleCHROMIUM");
  if (error == GL_NO_ERROR) {
    if (workarounds().validate_multisample_buffer_allocation) {
      if (!VerifyMultisampleRenderbufferIntegrity(
          renderbuffer->service_id(), impl_format)) {
        LOCAL_SET_GL_ERROR(
            GL_OUT_OF_MEMORY,
            "glRenderbufferStorageMultisampleCHROMIUM", "out of memory");
        return;
      }
    }

    renderbuffer_manager()->SetInfoAndInvalidate(renderbuffer, samples,
                                                 internalformat, width, height);
  }
}
