void EnableTabletMode(bool enable) {
  ash::Shell::Get()->tablet_mode_controller()->EnableTabletModeWindowManager(
      enable);
}
