  error::Error WillAccessBoundFramebufferForRead() {
    if (ShouldDeferReads())
      return error::kDeferCommandUntilLater;
    if (!offscreen_target_frame_buffer_.get() &&
        !framebuffer_state_.bound_read_framebuffer.get() &&
        !surface_->SetBackbufferAllocation(true))
      return error::kLostContext;
    return error::kNoError;
  }
