void GLES2DecoderImpl::DoClearBufferfv(GLenum buffer,
                                       GLint drawbuffer,
                                       const volatile GLfloat* value) {
  const char* func_name = "glClearBufferfv";
  if (!CheckBoundDrawFramebufferValid(func_name))
    return;
  ApplyDirtyState();

  if (buffer == GL_COLOR) {
    if (drawbuffer < 0 ||
        drawbuffer >= static_cast<GLint>(group_->max_draw_buffers())) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "invalid drawBuffer");
      return;
    }
    GLenum internal_format =
        GetBoundColorDrawBufferInternalFormat(drawbuffer);
    if (GLES2Util::IsIntegerFormat(internal_format)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
          "can only be called on float buffers");
      return;
    }
  } else {
    DCHECK(buffer == GL_DEPTH);
    if (drawbuffer != 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "invalid drawBuffer");
      return;
    }
    if (!BoundFramebufferHasDepthAttachment()) {
      return;
    }
  }
  MarkDrawBufferAsCleared(buffer, drawbuffer);
  api()->glClearBufferfvFn(buffer, drawbuffer,
                           const_cast<const GLfloat*>(value));
}
