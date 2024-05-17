X11SurfaceFactory::X11SurfaceFactory() {
  glx_implementation_.reset(new GLOzoneGLX());
  egl_implementation_.reset(new GLOzoneEGLX11());
}
