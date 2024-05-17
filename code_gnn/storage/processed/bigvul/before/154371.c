  bool SupportsSeparateFramebufferBinds() const {
    return (feature_info_->feature_flags().chromium_framebuffer_multisample ||
            feature_info_->IsWebGL2OrES3Context());
  }
