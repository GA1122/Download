void WindowTreeHostManager::PreDisplayConfigurationChange(bool clear_focus) {
  FOR_EACH_OBSERVER(Observer, observers_, OnDisplayConfigurationChanging());
  focus_activation_store_->Store(clear_focus);
  display::Screen* screen = display::Screen::GetScreen();
  gfx::Point point_in_screen = screen->GetCursorScreenPoint();
  cursor_location_in_screen_coords_for_restore_ = point_in_screen;

  display::Display display = screen->GetDisplayNearestPoint(point_in_screen);
  cursor_display_id_for_restore_ = display.id();

  gfx::Point point_in_native = point_in_screen;
  aura::Window* root_window = GetRootWindowForDisplayId(display.id());
  ::wm::ConvertPointFromScreen(root_window, &point_in_native);
  root_window->GetHost()->ConvertPointToNativeScreen(&point_in_native);
  cursor_location_in_native_coords_for_restore_ = point_in_native;
}
