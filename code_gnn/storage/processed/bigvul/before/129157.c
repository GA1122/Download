GLenum Framebuffer::GetDrawBuffer(GLenum draw_buffer) const {
  GLsizei index = static_cast<GLsizei>(
      draw_buffer - GL_DRAW_BUFFER0_ARB);
  CHECK(index >= 0 &&
        index < static_cast<GLsizei>(manager_->max_draw_buffers_));
  return draw_buffers_[index];
}
