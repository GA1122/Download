void GLES2DecoderImpl::AdjustDrawBuffers() {
  if (!SupportsDrawBuffers()) {
    return;
  }
  Framebuffer* framebuffer = framebuffer_state_.bound_draw_framebuffer.get();
  if (framebuffer) {
    framebuffer->AdjustDrawBuffers();
  }
}
