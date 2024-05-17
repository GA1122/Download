mojom::XRGamepadButtonPtr GetMojomGamepadButton(
    const GamepadBuilder::ButtonData& data) {
  auto ret = mojom::XRGamepadButton::New();
  ret->touched = data.touched;
  ret->pressed = data.pressed;
  ret->value = data.value;

  return ret;
}
