void DesktopWindowTreeHostX11::SetFullscreen(bool fullscreen) {
  if (is_fullscreen_ == fullscreen)
    return;
  is_fullscreen_ = fullscreen;
  if (is_fullscreen_)
    delayed_resize_task_.Cancel();

  bool unmaximize_and_remaximize = !fullscreen && IsMaximized() &&
                                   ui::GuessWindowManager() == ui::WM_METACITY;

  if (unmaximize_and_remaximize)
    Restore();
  SetWMSpecState(fullscreen, gfx::GetAtom("_NET_WM_STATE_FULLSCREEN"),
                 x11::None);
  if (unmaximize_and_remaximize)
    Maximize();

  if (fullscreen) {
    restored_bounds_in_pixels_ = bounds_in_pixels_;
    const display::Display display =
        display::Screen::GetScreen()->GetDisplayNearestWindow(window());
    bounds_in_pixels_ = ToPixelRect(display.bounds());
  } else {
    bounds_in_pixels_ = restored_bounds_in_pixels_;
  }
  OnHostMovedInPixels(bounds_in_pixels_.origin());
  OnHostResizedInPixels(bounds_in_pixels_.size());

  if (ui::HasWMSpecProperty(window_properties_,
                            gfx::GetAtom("_NET_WM_STATE_FULLSCREEN")) ==
      fullscreen) {
    Relayout();
    ResetWindowRegion();
  }
}
