  void SendCtrlKey(ui::KeyboardCode key) {
    ui::test::EventGenerator event_generator(Shell::GetPrimaryRootWindow());
    event_generator.PressKey(ui::VKEY_CONTROL, ui::EF_CONTROL_DOWN);
    event_generator.PressKey(key, ui::EF_CONTROL_DOWN);
    event_generator.ReleaseKey(key, ui::EF_CONTROL_DOWN);
    event_generator.ReleaseKey(ui::VKEY_CONTROL, ui::EF_NONE);
  }
