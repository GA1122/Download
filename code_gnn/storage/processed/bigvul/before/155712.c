std::map<vr::EVRButtonId, GamepadBuilder::ButtonData> GetAxesButtons(
    vr::IVRSystem* vr_system,
    const vr::VRControllerState_t& controller_state,
    uint64_t supported_buttons,
    uint32_t controller_id) {
  std::map<vr::EVRButtonId, GamepadBuilder::ButtonData> button_data_map;

  for (uint32_t j = 0; j < vr::k_unControllerStateAxisCount; ++j) {
    int32_t axis_type = vr_system->GetInt32TrackedDeviceProperty(
        controller_id,
        static_cast<vr::TrackedDeviceProperty>(vr::Prop_Axis0Type_Int32 + j));

    GamepadBuilder::ButtonData button_data;

    double x_axis = controller_state.rAxis[j].x;
    double y_axis = -controller_state.rAxis[j].y;

    switch (axis_type) {
      case vr::k_eControllerAxis_Joystick:
        x_axis = std::fabs(x_axis) < kJoystickDeadzone ? 0 : x_axis;
        y_axis = std::fabs(y_axis) < kJoystickDeadzone ? 0 : y_axis;
        FALLTHROUGH;
      case vr::k_eControllerAxis_TrackPad: {
        button_data.has_both_axes = true;
        button_data.x_axis = x_axis;
        button_data.y_axis = y_axis;
        vr::EVRButtonId button_id = GetAxisId(j);
        GamepadButton button;
        if (TryGetGamepadButton(controller_state, supported_buttons, button_id,
                                &button)) {
          button_data.touched = button.touched;
          button_data.pressed = button.pressed;
          button_data.value = button.value;
          button_data_map[button_id] = button_data;
        }
      } break;
      case vr::k_eControllerAxis_Trigger: {
        GamepadButton button;
        GamepadBuilder::ButtonData button_data;
        vr::EVRButtonId button_id = GetAxisId(j);
        if (TryGetGamepadButton(controller_state, supported_buttons, button_id,
                                &button)) {
          button_data.touched = button.touched;
          button_data.pressed = button.pressed;
          button_data.value = x_axis;
          button_data_map[button_id] = button_data;
        }
      } break;
    }
  }

  return button_data_map;
}
