void GamepadBuilder::AddAxes(const ButtonData& data) {
  DCHECK(data.has_both_axes);
  AddAxis(data.x_axis);
  AddAxis(data.y_axis);
}
