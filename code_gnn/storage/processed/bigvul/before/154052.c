void GLES2DecoderImpl::DoRenderbufferStorage(
  GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
  Renderbuffer* renderbuffer =
      GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glRenderbufferStorage", "no renderbuffer bound");
    return;
  }

  if (width > renderbuffer_manager()->max_renderbuffer_size() ||
      height > renderbuffer_manager()->max_renderbuffer_size()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glRenderbufferStorage", "dimensions too large");
    return;
  }

  uint32_t estimated_size = 0;
  if (!renderbuffer_manager()->ComputeEstimatedRenderbufferSize(
           width, height, 1, internalformat, &estimated_size)) {
    LOCAL_SET_GL_ERROR(
        GL_OUT_OF_MEMORY, "glRenderbufferStorage", "dimensions too large");
    return;
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glRenderbufferStorage");
  RenderbufferStorageMultisampleWithWorkaround(
      target, 0,
      renderbuffer_manager()->InternalRenderbufferFormatToImplFormat(
          internalformat),
      width, height, kDoNotForce);
  GLenum error = LOCAL_PEEK_GL_ERROR("glRenderbufferStorage");
  if (error == GL_NO_ERROR) {
    renderbuffer_manager()->SetInfoAndInvalidate(renderbuffer, 0,
                                                 internalformat, width, height);
  }
}
