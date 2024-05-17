void ShellSurface::Move() {
  TRACE_EVENT0("exo", "ShellSurface::Move");

  if (widget_ && !widget_->movement_disabled())
    AttemptToStartDrag(HTCAPTION);
}
