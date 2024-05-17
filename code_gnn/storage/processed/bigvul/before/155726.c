void GamepadBuilder::AddButton(const ButtonData& data) {
  AddButton(GamepadButton(data.pressed, data.touched, data.value));
  if (data.has_both_axes)
    AddAxes(data);
}
