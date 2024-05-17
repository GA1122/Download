bool GLES2DecoderPassthroughImpl::IsEmulatedFramebufferBound(
    GLenum target) const {
  if (!emulated_back_buffer_) {
    return false;
  }

  if ((target == GL_FRAMEBUFFER_EXT || target == GL_DRAW_FRAMEBUFFER) &&
      bound_draw_framebuffer_ == 0) {
    return true;
  }

  if (target == GL_READ_FRAMEBUFFER && bound_read_framebuffer_ == 0) {
    return true;
  }

  return false;
}
