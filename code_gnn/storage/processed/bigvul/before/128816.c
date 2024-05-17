bool GLSurfaceOzoneEGL::ReinitializeNativeSurface() {
  std::unique_ptr<ui::ScopedMakeCurrent> scoped_make_current;
  GLContext* current_context = GLContext::GetCurrent();
  bool was_current = current_context && current_context->IsCurrent(this);
  if (was_current) {
    scoped_make_current.reset(new ui::ScopedMakeCurrent(current_context, this));
  }

  Destroy();
  ozone_surface_ = ui::OzonePlatform::GetInstance()
                       ->GetSurfaceFactoryOzone()
                       ->CreateEGLSurfaceForWidget(widget_);
  if (!ozone_surface_) {
    LOG(ERROR) << "Failed to create native surface.";
    return false;
  }

  window_ = ozone_surface_->GetNativeWindow();
  if (!Initialize(format_)) {
    LOG(ERROR) << "Failed to initialize.";
    return false;
  }

  return true;
}
