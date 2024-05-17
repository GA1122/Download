void WebGL2RenderingContextBase::clearBufferuiv(
    GLenum buffer,
    GLint drawbuffer,
    MaybeShared<DOMUint32Array> value,
    GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferuiv", buffer, value.View()->length(),
                           src_offset))
    return;

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());

  ContextGL()->ClearBufferuiv(buffer, drawbuffer,
                              value.View()->DataMaybeShared() + src_offset);
}
