bool GLES2DecoderImpl::BoundFramebufferAllowsChangesToAlphaChannel() {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (framebuffer)
    return framebuffer->HasAlphaMRT();
  if (back_buffer_draw_buffer_ == GL_NONE)
    return false;
  if (offscreen_target_frame_buffer_.get()) {
    GLenum format = offscreen_target_color_format_;
    return (format == GL_RGBA || format == GL_RGBA8) &&
           offscreen_buffer_should_have_alpha_;
  }
  return (back_buffer_color_format_ == GL_RGBA ||
          back_buffer_color_format_ == GL_RGBA8);
}
