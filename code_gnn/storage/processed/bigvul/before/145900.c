  void SendKey(ui::KeyboardCode key) {
    ui::test::EventGenerator event_generator(Shell::GetPrimaryRootWindow());
    event_generator.PressKey(key, 0);
    event_generator.ReleaseKey(key, 0);
  }
