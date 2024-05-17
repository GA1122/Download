  bool TryAddButton(vr::EVRButtonId button_id) {
    GamepadButton button;
    if (TryGetGamepadButton(controller_state_, supported_buttons_, button_id,
                            &button)) {
      AddButton(button);
      return true;
    }

    return false;
   }
