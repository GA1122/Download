void WebGL2RenderingContextBase::clearBufferiv(GLenum buffer,
                                               GLint drawbuffer,
                                               const Vector<GLint>& value,
                                               GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferiv", buffer, value.size(), src_offset))
    return;

  ContextGL()->ClearBufferiv(buffer, drawbuffer, value.data() + src_offset);
}
