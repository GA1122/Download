void DesktopWindowTreeHostX11::Show(ui::WindowShowState show_state,
                                    const gfx::Rect& restore_bounds) {
  if (compositor())
    SetVisible(true);

  if (!window_mapped_in_client_ || IsMinimized())
    MapWindow(show_state);

  switch (show_state) {
    case ui::SHOW_STATE_MAXIMIZED:
      Maximize();
      if (!restore_bounds.IsEmpty()) {
        restored_bounds_in_pixels_ = ToPixelRect(restore_bounds);
      }

      break;
    case ui::SHOW_STATE_MINIMIZED:
      Minimize();
      break;
    case ui::SHOW_STATE_FULLSCREEN:
      SetFullscreen(true);
      break;
    default:
      break;
  }

  native_widget_delegate_->AsWidget()->SetInitialFocus(show_state);

  content_window()->Show();
}
