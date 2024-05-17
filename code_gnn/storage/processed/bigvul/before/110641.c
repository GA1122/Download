  bool ShouldDeferDraws() {
    return !offscreen_target_frame_buffer_.get() &&
           bound_draw_framebuffer_ == NULL &&
           surface_->DeferDraws();
  }
