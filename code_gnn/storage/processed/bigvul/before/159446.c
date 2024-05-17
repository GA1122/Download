void WebGL2RenderingContextBase::clearBufferuiv(GLenum buffer,
                                                GLint drawbuffer,
                                                const Vector<GLuint>& value,
                                                GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferuiv", buffer, value.size(), src_offset))
    return;

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());

  ContextGL()->ClearBufferuiv(buffer, drawbuffer, value.data() + src_offset);
}
