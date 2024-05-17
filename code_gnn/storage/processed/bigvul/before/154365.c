  bool ShouldDeferDraws() {
    return !offscreen_target_frame_buffer_.get() &&
           framebuffer_state_.bound_draw_framebuffer.get() == nullptr &&
           surface_->DeferDraws();
  }
