  gfx::Vector2d GetBoundFramebufferDrawOffset() const {
    if (GetBoundDrawFramebuffer() || offscreen_target_frame_buffer_.get())
      return gfx::Vector2d();
    return surface_->GetDrawOffset();
  }
