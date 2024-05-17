static void RebindCurrentFramebuffer(
    GLenum target,
    FramebufferManager::FramebufferInfo* info,
    FrameBuffer* offscreen_frame_buffer) {
  GLuint framebuffer_id = info ? info->service_id() : 0;

  if (framebuffer_id == 0 && offscreen_frame_buffer) {
    framebuffer_id = offscreen_frame_buffer->id();
  }

  glBindFramebufferEXT(target, framebuffer_id);
}
