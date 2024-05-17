void Framebuffer::SetDrawBuffers(GLsizei n, const GLenum* bufs) {
  DCHECK(n <= static_cast<GLsizei>(manager_->max_draw_buffers_));
  for (GLsizei i = 0; i < n; ++i)
     draw_buffers_[i] = bufs[i];
 }
