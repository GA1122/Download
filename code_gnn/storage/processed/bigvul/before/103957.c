bool GLES2DecoderImpl::BoundFramebufferHasDepthAttachment() {
  if (bound_draw_framebuffer_) {
    return bound_draw_framebuffer_->HasDepthAttachment();
  }
  if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_depth_format_ != 0;
  }
  return back_buffer_has_depth_;
}
