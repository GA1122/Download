gfx::Rect DesktopWindowTreeHostX11::GetWorkAreaBoundsInScreen() const {
  return display::Screen::GetScreen()
      ->GetDisplayNearestWindow(const_cast<aura::Window*>(window()))
      .work_area();
}
