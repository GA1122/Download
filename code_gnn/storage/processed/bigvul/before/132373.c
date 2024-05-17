void VirtualKeyboardController::UpdateKeyboardEnabled() {
  if (!IsSmartVirtualKeyboardEnabled()) {
    SetKeyboardEnabled(Shell::GetInstance()
                           ->maximize_mode_controller()
                           ->IsMaximizeModeWindowManagerEnabled());
    return;
  }
  bool ignore_internal_keyboard = Shell::GetInstance()
                                      ->maximize_mode_controller()
                                      ->IsMaximizeModeWindowManagerEnabled();
  bool is_internal_keyboard_active =
      has_internal_keyboard_ && !ignore_internal_keyboard;
  SetKeyboardEnabled(!is_internal_keyboard_active && has_touchscreen_ &&
                     (!has_external_keyboard_ || ignore_external_keyboard_));
  ash::Shell::GetInstance()
      ->system_tray_notifier()
      ->NotifyVirtualKeyboardSuppressionChanged(!is_internal_keyboard_active &&
                                                has_touchscreen_ &&
                                                has_external_keyboard_);
}
