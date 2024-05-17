EGLDisplay RenderingHelper::PlatformGetDisplay() {
  return eglGetDisplay(EGL_DEFAULT_DISPLAY);
}
