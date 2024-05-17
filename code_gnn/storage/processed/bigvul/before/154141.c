GLenum GLES2DecoderImpl::GetBoundFramebufferDepthFormat(
    GLenum target) {
  DCHECK(target == GL_DRAW_FRAMEBUFFER || target == GL_READ_FRAMEBUFFER ||
         target == GL_FRAMEBUFFER);
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (framebuffer) {
    return framebuffer->GetDepthFormat();
  } else {   
    if (offscreen_target_frame_buffer_.get()) {
      return offscreen_target_depth_format_;
    }
    if (back_buffer_has_depth_)
      return GL_DEPTH;
    return 0;
  }
}
