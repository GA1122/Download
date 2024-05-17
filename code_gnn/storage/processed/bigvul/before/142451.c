bool IsTabletModeEnabled() {
  if (!Shell::Get()->tablet_mode_controller())
    return false;

  return Shell::Get()
      ->tablet_mode_controller()
      ->IsTabletModeWindowManagerEnabled();
}
