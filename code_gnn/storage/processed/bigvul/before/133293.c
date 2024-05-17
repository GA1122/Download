void HandleRotateScreen() {
  if (Shell::GetInstance()->display_manager()->IsInUnifiedMode())
    return;

  base::RecordAction(UserMetricsAction("Accel_Rotate_Window"));
  gfx::Point point = display::Screen::GetScreen()->GetCursorScreenPoint();
  display::Display display =
      display::Screen::GetScreen()->GetDisplayNearestPoint(point);
  const DisplayInfo& display_info =
      Shell::GetInstance()->display_manager()->GetDisplayInfo(display.id());
  ScreenRotationAnimator(display.id())
      .Rotate(GetNextRotation(display_info.GetActiveRotation()),
              display::Display::ROTATION_SOURCE_USER);
}
