FramebufferManager::FramebufferManager(
    uint32 max_draw_buffers, uint32 max_color_attachments)
    : framebuffer_state_change_count_(1),
      framebuffer_count_(0),
      have_context_(true),
      max_draw_buffers_(max_draw_buffers),
      max_color_attachments_(max_color_attachments) {
  DCHECK_GT(max_draw_buffers_, 0u);
  DCHECK_GT(max_color_attachments_, 0u);
}
