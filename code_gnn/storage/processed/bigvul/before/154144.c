GLenum GLES2DecoderImpl::GetBoundFramebufferStencilFormat(
    GLenum target) {
  DCHECK(target == GL_DRAW_FRAMEBUFFER || target == GL_READ_FRAMEBUFFER ||
         target == GL_FRAMEBUFFER);
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (framebuffer) {
    return framebuffer->GetStencilFormat();
  } else {   
    if (offscreen_target_frame_buffer_.get()) {
      return offscreen_target_stencil_format_;
    }
    if (back_buffer_has_stencil_)
      return GL_STENCIL;
    return 0;
  }
}
