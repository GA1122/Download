void WebGL2RenderingContextBase::clearBufferiv(GLenum buffer,
                                               GLint drawbuffer,
                                               MaybeShared<DOMInt32Array> value,
                                               GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferiv", buffer, value.View()->length(),
                           src_offset))
    return;

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());

  ContextGL()->ClearBufferiv(buffer, drawbuffer,
                             value.View()->DataMaybeShared() + src_offset);
  UpdateBuffersToAutoClear(kClearBufferiv, buffer, drawbuffer);
}
