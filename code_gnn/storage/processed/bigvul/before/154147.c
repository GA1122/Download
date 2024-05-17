GLuint GLES2DecoderImpl::GetBoundReadFramebufferServiceId() {
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (framebuffer) {
    return framebuffer->service_id();
  }
  if (offscreen_resolved_frame_buffer_.get()) {
    return offscreen_resolved_frame_buffer_->id();
  }
  if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_frame_buffer_->id();
  }
  if (surface_.get()) {
    return surface_->GetBackingFramebufferObject();
  }
  return 0;
}
