ui::WindowShowState ConvertToWindowShowState(windows::WindowState state) {
  switch (state) {
    case windows::WINDOW_STATE_NORMAL:
    case windows::WINDOW_STATE_DOCKED:
      return ui::SHOW_STATE_NORMAL;
    case windows::WINDOW_STATE_MINIMIZED:
      return ui::SHOW_STATE_MINIMIZED;
    case windows::WINDOW_STATE_MAXIMIZED:
      return ui::SHOW_STATE_MAXIMIZED;
    case windows::WINDOW_STATE_FULLSCREEN:
    case windows::WINDOW_STATE_LOCKED_FULLSCREEN:
      return ui::SHOW_STATE_FULLSCREEN;
    case windows::WINDOW_STATE_NONE:
      return ui::SHOW_STATE_DEFAULT;
  }
  NOTREACHED();
  return ui::SHOW_STATE_DEFAULT;
}
