EGLConfig GLSurfaceOzoneEGL::GetConfig() {
  if (!config_) {
    ui::EglConfigCallbacks callbacks = GetEglConfigCallbacks(GetDisplay());
    config_ = ozone_surface_->GetEGLSurfaceConfig(callbacks);
  }
  if (config_)
    return config_;
  return NativeViewGLSurfaceEGL::GetConfig();
}
