void RenderWidgetHostViewAura::UpdateCursorIfOverSelf() {
  const gfx::Point screen_point =
      gfx::Screen::GetScreenFor(GetNativeView())->GetCursorScreenPoint();
  aura::Window* root_window = window_->GetRootWindow();
  if (!root_window)
    return;

  gfx::Point root_window_point = screen_point;
  aura::client::ScreenPositionClient* screen_position_client =
      aura::client::GetScreenPositionClient(root_window);
  if (screen_position_client) {
    screen_position_client->ConvertPointFromScreen(
        root_window, &root_window_point);
  }

  if (root_window->GetEventHandlerForPoint(root_window_point) != window_)
    return;

  gfx::NativeCursor cursor = current_cursor_.GetNativeCursor();
  if (is_loading_ && cursor != ui::kCursorNone)
    cursor = ui::kCursorPointer;

  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root_window);
  if (cursor_client) {
    cursor_client->SetCursor(cursor);
  }
}
