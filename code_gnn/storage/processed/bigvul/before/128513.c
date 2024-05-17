void ShellSurface::SetPinned(bool pinned) {
  TRACE_EVENT1("exo", "ShellSurface::SetPinned", "pinned", pinned);

  if (!widget_)
    CreateShellSurfaceWidget(ui::SHOW_STATE_NORMAL);

  ScopedConfigure scoped_configure(this, true);
  if (pinned) {
    ash::wm::PinWindow(widget_->GetNativeWindow());
  } else {
    if (ash::wm::GetWindowState(widget_->GetNativeWindow())->IsPinned())
      widget_->Restore();
  }
}
