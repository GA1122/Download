  void SendKeyEvent(ui::KeyboardCode key_code,
                    bool alt,
                    bool shift,
                    bool control_or_command,
                    bool caps_lock) {
    bool control = control_or_command;
    bool command = false;

    if (TestingNativeMac())
      std::swap(control, command);

    int flags = (shift ? ui::EF_SHIFT_DOWN : 0) |
                (control ? ui::EF_CONTROL_DOWN : 0) |
                (alt ? ui::EF_ALT_DOWN : 0) |
                (command ? ui::EF_COMMAND_DOWN : 0) |
                (caps_lock ? ui::EF_CAPS_LOCK_ON : 0);

    SendKeyPress(key_code, flags);
  }
