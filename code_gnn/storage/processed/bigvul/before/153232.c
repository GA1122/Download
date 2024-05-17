gfx::Transform DesktopWindowTreeHostX11::GetRootTransform() const {
  display::Display display = display::Screen::GetScreen()->GetPrimaryDisplay();
  if (IsVisible()) {
    aura::Window* win = const_cast<aura::Window*>(window());
    display = display::Screen::GetScreen()->GetDisplayNearestWindow(win);
  }

  float scale = display.device_scale_factor();
  gfx::Transform transform;
  transform.Scale(scale, scale);
  return transform;
}
