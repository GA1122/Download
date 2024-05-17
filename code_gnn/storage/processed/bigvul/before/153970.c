void GLES2DecoderImpl::DoClearBufferuiv(GLenum buffer,
                                        GLint drawbuffer,
                                        const volatile GLuint* value) {
  const char* func_name = "glClearBufferuiv";
  if (!CheckBoundDrawFramebufferValid(func_name))
    return;
  ApplyDirtyState();

  if (drawbuffer < 0 ||
      drawbuffer >= static_cast<GLint>(group_->max_draw_buffers())) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "invalid drawBuffer");
    return;
  }
  GLenum internal_format =
      GetBoundColorDrawBufferInternalFormat(drawbuffer);
  if (!GLES2Util::IsUnsignedIntegerFormat(internal_format)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "can only be called on unsigned integer buffers");
    return;
  }
  MarkDrawBufferAsCleared(buffer, drawbuffer);
  api()->glClearBufferuivFn(buffer, drawbuffer,
                            const_cast<const GLuint*>(value));
}
