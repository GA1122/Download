void GLES2DecoderImpl::UpdateFramebufferSRGB(Framebuffer* framebuffer) {
  bool needs_enable_disable_framebuffer_srgb = false;
  bool enable_framebuffer_srgb = true;
  if (feature_info_->feature_flags().ext_srgb_write_control) {
    needs_enable_disable_framebuffer_srgb = true;
    enable_framebuffer_srgb &= state_.GetEnabled(GL_FRAMEBUFFER_SRGB);
  }
  if (feature_info_->feature_flags().desktop_srgb_support) {
    needs_enable_disable_framebuffer_srgb = true;
    enable_framebuffer_srgb &= framebuffer && framebuffer->HasSRGBAttachments();
  }
  if (needs_enable_disable_framebuffer_srgb)
    state_.EnableDisableFramebufferSRGB(enable_framebuffer_srgb);
}
