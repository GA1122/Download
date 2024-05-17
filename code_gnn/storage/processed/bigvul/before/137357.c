  void SendAlternatePaste() {
    if (TestingNativeMac())
      SendKeyEvent(ui::VKEY_V, false, true);
    else
      SendKeyEvent(ui::VKEY_INSERT, true, false);
  }
