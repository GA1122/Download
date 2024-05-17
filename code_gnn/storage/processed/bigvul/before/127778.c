EGLConfig GLSurfaceEGLOzoneX11::GetConfig() {
  const int kBufferSizeOffset = 1;
  const int kAlphaSizeOffset = 3;
  EGLint config_attribs[] = {EGL_BUFFER_SIZE,
                             ~0,   
                             EGL_ALPHA_SIZE,
                             8,
                             EGL_BLUE_SIZE,
                             8,
                             EGL_GREEN_SIZE,
                             8,
                             EGL_RED_SIZE,
                             8,
                             EGL_RENDERABLE_TYPE,
                             EGL_OPENGL_ES2_BIT,
                             EGL_SURFACE_TYPE,
                             EGL_WINDOW_BIT,
                             EGL_NONE};

  XWindowAttributes win_attribs;
  if (XGetWindowAttributes(gfx::GetXDisplay(), window_, &win_attribs)) {
    config_attribs[kBufferSizeOffset] = win_attribs.depth;
  }

  EGLDisplay display = GetDisplay();

  EGLConfig config;
  EGLint num_configs;
  if (!eglChooseConfig(display, config_attribs, &config, 1, &num_configs)) {
    LOG(ERROR) << "eglChooseConfig failed with error "
               << GetLastEGLErrorString();
    return nullptr;
  }

  if (num_configs > 0) {
    EGLint config_depth;
    if (!eglGetConfigAttrib(display, config, EGL_BUFFER_SIZE, &config_depth)) {
      LOG(ERROR) << "eglGetConfigAttrib failed with error "
                 << GetLastEGLErrorString();
      return nullptr;
    }
    if (config_depth == config_attribs[kBufferSizeOffset]) {
      return config;
    }
  }

  config_attribs[kAlphaSizeOffset] = 0;
  if (!eglChooseConfig(display, config_attribs, &config, 1, &num_configs)) {
    LOG(ERROR) << "eglChooseConfig failed with error "
               << GetLastEGLErrorString();
    return nullptr;
  }

  if (num_configs == 0) {
    LOG(ERROR) << "No suitable EGL configs found.";
    return nullptr;
  }
  return config;
}
