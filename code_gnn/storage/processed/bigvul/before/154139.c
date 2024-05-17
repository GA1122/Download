GLuint GLES2DecoderImpl::GetBoundDrawFramebufferServiceId() const {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (framebuffer) {
    return framebuffer->service_id();
  }
  if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_frame_buffer_->id();
  }
  if (surface_.get()) {
    return surface_->GetBackingFramebufferObject();
  }
  return 0;
}
