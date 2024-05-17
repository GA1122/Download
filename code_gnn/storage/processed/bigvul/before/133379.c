void WindowTreeHostManager::UpdateMouseLocationAfterDisplayChange() {

  gfx::Point point_in_screen =
      display::Screen::GetScreen()->GetCursorScreenPoint();
  gfx::Point target_location_in_native;
  int64_t closest_distance_squared = -1;
  DisplayManager* display_manager = GetDisplayManager();

  aura::Window* dst_root_window = nullptr;
  for (size_t i = 0; i < display_manager->GetNumDisplays(); ++i) {
    const display::Display& display = display_manager->GetDisplayAt(i);
    const DisplayInfo display_info =
        display_manager->GetDisplayInfo(display.id());
    aura::Window* root_window = GetRootWindowForDisplayId(display.id());
    if (display_info.bounds_in_native().Contains(
            cursor_location_in_native_coords_for_restore_)) {
      dst_root_window = root_window;
      target_location_in_native = cursor_location_in_native_coords_for_restore_;
      break;
    }
    gfx::Point center = display.bounds().CenterPoint();

    int64_t distance_squared = (center - point_in_screen).LengthSquared();
    if (closest_distance_squared < 0 ||
        closest_distance_squared > distance_squared) {
      aura::Window* root_window = GetRootWindowForDisplayId(display.id());
      ::wm::ConvertPointFromScreen(root_window, &center);
      root_window->GetHost()->ConvertPointToNativeScreen(&center);
      dst_root_window = root_window;
      target_location_in_native = center;
      closest_distance_squared = distance_squared;
    }
  }

  gfx::Point target_location_in_root = target_location_in_native;
  dst_root_window->GetHost()->ConvertPointFromNativeScreen(
      &target_location_in_root);

#if defined(USE_OZONE)
  gfx::Point target_location_in_screen = target_location_in_root;
  ::wm::ConvertPointToScreen(dst_root_window, &target_location_in_screen);
  const display::Display& target_display =
      display_manager->FindDisplayContainingPoint(target_location_in_screen);
  if (!target_display.is_valid())
    return;
  int64_t target_display_id = target_display.id();

  if (target_location_in_native !=
          cursor_location_in_native_coords_for_restore_ ||
      target_display_id != cursor_display_id_for_restore_) {
    dst_root_window->MoveCursorTo(target_location_in_root);
  } else if (target_location_in_screen !=
             cursor_location_in_screen_coords_for_restore_) {
    Shell::GetInstance()->cursor_manager()->SetDisplay(target_display);

    dst_root_window->GetHost()->dispatcher()->OnCursorMovedToRootLocation(
        target_location_in_root);
  }
#else
  dst_root_window->MoveCursorTo(target_location_in_root);
#endif
}
