void GamepadBuilder::AddAxis(double value) {
  DCHECK_LT(gamepad_.axes_length, Gamepad::kAxesLengthCap);
  gamepad_.axes[gamepad_.axes_length++] = ApplyAxisDeadzoneToValue(value);
}
