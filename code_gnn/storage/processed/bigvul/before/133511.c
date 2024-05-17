ScreenPositionController* GetScreenPositionController() {
  ShellTestApi test_api(Shell::GetInstance());
  return test_api.screen_position_controller();
}
