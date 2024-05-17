void WebGL2RenderingContextBase::clearBufferiv(GLenum buffer,
                                               GLint drawbuffer,
                                               MaybeShared<DOMInt32Array> value,
                                               GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferiv", buffer, value.View()->length(),
                           src_offset))
    return;

  ContextGL()->ClearBufferiv(buffer, drawbuffer,
                             value.View()->DataMaybeShared() + src_offset);
}
