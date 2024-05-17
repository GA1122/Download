mojom::XRGamepadButtonPtr GetMojomGamepadButton(const GamepadButton& data) {
  auto ret = mojom::XRGamepadButton::New();
  ret->touched = data.touched;
  ret->pressed = data.pressed;
  ret->value = data.value;

  return ret;
}
