bool RenderWidgetHostViewAura::LockMouse() {
  aura::Window* root_window = window_->GetRootWindow();
  if (!root_window)
    return false;

  if (mouse_locked_)
    return true;

  mouse_locked_ = true;
#if !defined(OS_WIN)
  window_->SetCapture();
#endif
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root_window);
  if (cursor_client) {
    cursor_client->HideCursor();
    cursor_client->LockCursor();
  }

  if (ShouldMoveToCenter()) {
    synthetic_move_sent_ = true;
    window_->MoveCursorTo(gfx::Rect(window_->bounds().size()).CenterPoint());
  }
  tooltip_disabler_.reset(new aura::client::ScopedTooltipDisabler(root_window));

  root_window->GetDispatcher()->host()->ConfineCursorToRootWindow();
  return true;
}
