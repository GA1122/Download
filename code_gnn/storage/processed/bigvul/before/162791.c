void ContextState::EnableDisableFramebufferSRGB(bool enable) {
  if (framebuffer_srgb_valid_ && framebuffer_srgb_ == enable)
    return;
  EnableDisable(GL_FRAMEBUFFER_SRGB, enable);
  framebuffer_srgb_ = enable;
  framebuffer_srgb_valid_ = true;
}
