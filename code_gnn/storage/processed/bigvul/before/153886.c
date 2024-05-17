bool GLES2DecoderImpl::BoundFramebufferHasStencilAttachment() {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  if (framebuffer) {
    return framebuffer->HasStencilAttachment();
  }
  if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_stencil_format_ != 0 ||
           offscreen_target_depth_format_ == GL_DEPTH24_STENCIL8;
  }
  return back_buffer_has_stencil_;
}
