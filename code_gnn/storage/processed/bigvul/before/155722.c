bool TryGetGamepadButton(const vr::VRControllerState_t& controller_state,
                         uint64_t supported_buttons,
                         vr::EVRButtonId button_id,
                         GamepadButton* button) {
  uint64_t button_mask = vr::ButtonMaskFromId(button_id);
  if ((supported_buttons & button_mask) != 0) {
    bool button_pressed = (controller_state.ulButtonPressed & button_mask) != 0;
    bool button_touched = (controller_state.ulButtonTouched & button_mask) != 0;
    button->touched = button_touched || button_pressed;
    button->pressed = button_pressed;
    button->value = button_pressed ? 1.0 : 0.0;
    return true;
  }

  return false;
}
