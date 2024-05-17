void ShellSurface::Minimize() {
  TRACE_EVENT0("exo", "ShellSurface::Minimize");

  if (!widget_)
    return;

  ScopedConfigure scoped_configure(this, true);
  widget_->Minimize();
}
