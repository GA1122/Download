  void SendAlternateCopy() {
    if (TestingNativeMac())
      SendKeyEvent(ui::VKEY_C, false, true);
    else
      SendKeyEvent(ui::VKEY_INSERT, false, true);
  }
