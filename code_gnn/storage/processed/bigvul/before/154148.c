gfx::Size GLES2DecoderImpl::GetBoundReadFramebufferSize() {
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (framebuffer) {
    return framebuffer->GetFramebufferValidSize();
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_size_;
  } else {
    return surface_->GetSize();
  }
}
