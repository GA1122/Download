GLsizei GLES2DecoderImpl::GetBoundFramebufferSamples(GLenum target) {
  DCHECK(target == GL_DRAW_FRAMEBUFFER || target == GL_READ_FRAMEBUFFER ||
         target == GL_FRAMEBUFFER);
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (framebuffer) {
    return framebuffer->GetSamples();
  } else {   
    if (offscreen_target_frame_buffer_.get()) {
      return offscreen_target_samples_;
    }
    return 0;
  }
}
