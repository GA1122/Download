EGLDisplay RenderingHelper::PlatformGetDisplay() {
  return eglGetDisplay(x_display_);
}
