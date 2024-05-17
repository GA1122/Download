Framebuffer::Framebuffer(
    FramebufferManager* manager, GLuint service_id)
    : manager_(manager),
      deleted_(false),
      service_id_(service_id),
      has_been_bound_(false),
      framebuffer_complete_state_count_id_(0) {
  manager->StartTracking(this);
  DCHECK_GT(manager->max_draw_buffers_, 0u);
  draw_buffers_.reset(new GLenum[manager->max_draw_buffers_]);
  draw_buffers_[0] = GL_COLOR_ATTACHMENT0;
  for (uint32 i = 1; i < manager->max_draw_buffers_; ++i)
    draw_buffers_[i] = GL_NONE;
}
