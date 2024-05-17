gfx::Size GLES2DecoderImpl::GetBoundDrawFramebufferSize() {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (framebuffer) {
    return framebuffer->GetFramebufferValidSize();
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_size_;
  } else {
    return surface_->GetSize();
  }
}
