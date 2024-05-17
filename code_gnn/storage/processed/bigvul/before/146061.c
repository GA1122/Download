void WebGL2RenderingContextBase::clearBufferfv(GLenum buffer,
                                               GLint drawbuffer,
                                               const Vector<GLfloat>& value,
                                               GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferfv", buffer, value.size(), src_offset))
    return;

  ContextGL()->ClearBufferfv(buffer, drawbuffer, value.data() + src_offset);
}
