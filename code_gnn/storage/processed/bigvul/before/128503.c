void ShellSurface::Resize(int component) {
  TRACE_EVENT1("exo", "ShellSurface::Resize", "component", component);

  if (widget_ && !widget_->movement_disabled())
    AttemptToStartDrag(component);
}
