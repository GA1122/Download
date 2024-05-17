void GamepadBuilder::AddButton(const GamepadButton& button) {
  DCHECK_LT(gamepad_.buttons_length, Gamepad::kButtonsLengthCap);
  gamepad_.buttons[gamepad_.buttons_length++] = button;
}
