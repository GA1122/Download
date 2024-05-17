void GamepadBuilder::SetAxisDeadzone(double deadzone) {
  DCHECK_GE(deadzone, 0);
  axis_deadzone_ = deadzone;
}
