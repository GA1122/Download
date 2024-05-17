void GLES2DecoderImpl::DoClearBufferiv(GLenum buffer,
                                       GLint drawbuffer,
                                       const volatile GLint* value) {
  const char* func_name = "glClearBufferiv";
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
    if (!GLES2Util::IsSignedIntegerFormat(internal_format)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
          "can only be called on signed integer buffers");
      return;
    }
  } else {
    DCHECK(buffer == GL_STENCIL);
    if (drawbuffer != 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "invalid drawBuffer");
      return;
    }
    if (!BoundFramebufferHasStencilAttachment()) {
      return;
    }
  }
  MarkDrawBufferAsCleared(buffer, drawbuffer);
  api()->glClearBufferivFn(buffer, drawbuffer, const_cast<const GLint*>(value));
}
