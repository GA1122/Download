  void SendWordEvent(ui::KeyboardCode key, bool shift) {
    bool alt = false;
    bool control = true;
    bool caps = false;
    if (TestingNativeMac()) {
      alt = true;
      control = false;
    }
    SendKeyEvent(key, alt, shift, control, caps);
  }
