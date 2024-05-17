void GLES2DecoderImpl::DoClearBufferfi(
    GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
  const char* func_name = "glClearBufferfi";
  if (!CheckBoundDrawFramebufferValid(func_name))
    return;
  ApplyDirtyState();

  if (drawbuffer != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, func_name, "invalid drawBuffer");
    return;
  }
  if (!BoundFramebufferHasDepthAttachment() &&
      !BoundFramebufferHasStencilAttachment()) {
    return;
  }
  MarkDrawBufferAsCleared(GL_DEPTH, drawbuffer);
  MarkDrawBufferAsCleared(GL_STENCIL, drawbuffer);
  api()->glClearBufferfiFn(buffer, drawbuffer, depth, stencil);
}
