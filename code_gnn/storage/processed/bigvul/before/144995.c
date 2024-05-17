void RenderWidgetHostViewAura::UpdateCursorIfOverSelf() {
  if (host_->GetProcess()->FastShutdownStarted())
    return;

  aura::Window* root_window = window_->GetRootWindow();
  if (!root_window)
    return;

  display::Screen* screen = display::Screen::GetScreen();
  DCHECK(screen);

  gfx::Point cursor_screen_point = screen->GetCursorScreenPoint();

#if !defined(OS_CHROMEOS)
  aura::Window* window_at_screen_point = screen->GetWindowAtScreenPoint(
      cursor_screen_point);
#if defined(OS_WIN)
  if (!window_at_screen_point && legacy_render_widget_host_HWND_) {
    HWND hwnd_at_point = ::WindowFromPoint(cursor_screen_point.ToPOINT());

    if (hwnd_at_point == legacy_render_widget_host_HWND_->hwnd())
      hwnd_at_point = legacy_render_widget_host_HWND_->GetParent();

    display::win::ScreenWin* screen_win =
        static_cast<display::win::ScreenWin*>(screen);
    window_at_screen_point = screen_win->GetNativeWindowFromHWND(
        hwnd_at_point);
  }
#endif   
  if (!window_at_screen_point ||
      (window_at_screen_point->GetRootWindow() != root_window)) {
    return;
  }
#endif   

  gfx::Point root_window_point = cursor_screen_point;
  aura::client::ScreenPositionClient* screen_position_client =
      aura::client::GetScreenPositionClient(root_window);
  if (screen_position_client) {
    screen_position_client->ConvertPointFromScreen(
        root_window, &root_window_point);
  }

  if (root_window->GetEventHandlerForPoint(root_window_point) != window_)
    return;

  gfx::NativeCursor cursor = current_cursor_.GetNativeCursor();
  if (is_loading_ && cursor != ui::CursorType::kNone)
    cursor = ui::Cursor(ui::CursorType::kPointer);

  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root_window);
  if (cursor_client) {
    cursor_client->SetCursor(cursor);
  }
}
