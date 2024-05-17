void GLES2DecoderImpl::DoRenderbufferStorageMultisampleAdvancedAMD(
    GLenum target,
    GLsizei samples,
    GLsizei storageSamples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  Renderbuffer* renderbuffer = GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glRenderbufferStorageMultisampleAdvancedAMD",
                       "no renderbuffer bound");
    return;
  }

  if (!ValidateRenderbufferStorageMultisampleAMD(
          samples, storageSamples, internalformat, width, height)) {
    return;
  }

  GLenum impl_format =
      renderbuffer_manager()->InternalRenderbufferFormatToImplFormat(
          internalformat);
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(
      "glRenderbufferStorageMultisampleAdvancedAMD");
  RenderbufferStorageMultisampleHelperAMD(
      target, samples, storageSamples, impl_format, width, height, kDoNotForce);
  GLenum error =
      LOCAL_PEEK_GL_ERROR("glRenderbufferStorageMultisampleAdvancedAMD");
  if (error == GL_NO_ERROR) {
    if (workarounds().validate_multisample_buffer_allocation) {
      if (!VerifyMultisampleRenderbufferIntegrity(renderbuffer->service_id(),
                                                  impl_format)) {
        LOCAL_SET_GL_ERROR(GL_OUT_OF_MEMORY,
                           "glRenderbufferStorageMultisampleAdvancedAMD",
                           "out of memory");
        return;
      }
    }

    renderbuffer_manager()->SetInfoAndInvalidate(renderbuffer, samples,
                                                 internalformat, width, height);
  }
}
