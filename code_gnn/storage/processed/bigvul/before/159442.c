void WebGL2RenderingContextBase::clearBufferfv(GLenum buffer,
                                               GLint drawbuffer,
                                               const Vector<GLfloat>& value,
                                               GLuint src_offset) {
  if (isContextLost() ||
      !ValidateClearBuffer("clearBufferfv", buffer, value.size(), src_offset))
    return;

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());

  ContextGL()->ClearBufferfv(buffer, drawbuffer, value.data() + src_offset);
  MarkContextChanged(kCanvasChanged);
}
