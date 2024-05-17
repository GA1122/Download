bool GLES2Implementation::IsChromiumFramebufferMultisampleAvailable() {
  return IsExtensionAvailableHelper("GL_CHROMIUM_framebuffer_multisample",
                                    &chromium_framebuffer_multisample_);
}
