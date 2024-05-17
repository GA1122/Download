  void SendKeyEvent(ui::KeyboardCode key_code,
                    bool shift,
                    bool control_or_command) {
    SendKeyEvent(key_code, false, shift, control_or_command, false);
  }
