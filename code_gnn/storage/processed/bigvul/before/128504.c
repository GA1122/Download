void ShellSurface::Restore() {
  TRACE_EVENT0("exo", "ShellSurface::Restore");

  if (!widget_)
    return;

  ScopedConfigure scoped_configure(this, true);
  widget_->Restore();
}
