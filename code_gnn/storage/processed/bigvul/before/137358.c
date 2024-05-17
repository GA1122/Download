  void SendHomeEvent(bool shift) {
    if (TestingNativeMac()) {
      SendKeyEvent(ui::VKEY_UP, shift  , true  );
      return;
    }
    SendKeyEvent(ui::VKEY_HOME, shift  , false  );
  }
