bool GLSurfaceEGLOzoneX11::Resize(const gfx::Size& size,
                                  float scale_factor,
                                  bool has_alpha) {
  if (size == GetSize())
    return true;

  size_ = size;

  eglWaitGL();
  XResizeWindow(gfx::GetXDisplay(), window_, size.width(), size.height());
  eglWaitNative(EGL_CORE_NATIVE_ENGINE);

  return true;
}
