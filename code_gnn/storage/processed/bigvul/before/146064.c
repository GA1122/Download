void WebGL2RenderingContextBase::clearBufferuiv(
    GLenum buffer,
    GLint drawbuffer,
    MaybeShared<DOMUint32Array> value,
    GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferuiv", buffer, value.View()->length(),
                           src_offset))
    return;

  ContextGL()->ClearBufferuiv(buffer, drawbuffer,
                              value.View()->DataMaybeShared() + src_offset);
}
