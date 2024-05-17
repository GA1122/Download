void RenderWidgetHostViewAura::UnlockMouse() {
  tooltip_disabler_.reset();

  aura::Window* root_window = window_->GetRootWindow();
  if (!mouse_locked_ || !root_window)
    return;

  mouse_locked_ = false;

#if !defined(OS_WIN)
  window_->ReleaseCapture();
#endif
  window_->MoveCursorTo(unlocked_mouse_position_);
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root_window);
  if (cursor_client) {
    cursor_client->UnlockCursor();
    cursor_client->ShowCursor();
  }

  host_->LostMouseLock();
  root_window->GetDispatcher()->host()->UnConfineCursor();
}
