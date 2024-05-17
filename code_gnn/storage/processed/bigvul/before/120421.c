  void SendKeyToPageAndWait(ui::KeyboardCode key) {
    test_delegate_.Reset();
    content::SimulateKeyPress(
        GetWebContents(), key, false, false, false, false);
    test_delegate_.Wait();
  }
