bool ShellSurface::WidgetHasHitTestMask() const {
  return surface_ ? surface_->HasHitTestMask() : false;
}
