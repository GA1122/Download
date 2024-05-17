  bool ShouldDeferReads() {
    return !offscreen_target_frame_buffer_.get() &&
           framebuffer_state_.bound_read_framebuffer.get() == NULL &&
           surface_->DeferDraws();
  }
