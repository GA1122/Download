void FramebufferManager::MarkAsComplete(
    Framebuffer* framebuffer) {
  DCHECK(framebuffer);
  framebuffer->MarkAsComplete(framebuffer_state_change_count_);
}
