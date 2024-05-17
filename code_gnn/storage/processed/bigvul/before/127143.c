  void SendKeyToPopupAndWait(ui::KeyboardCode key) {
    if (!external_delegate_) {
      SendKeyToPageAndWait(key);
      return;
    }

    content::NativeWebKeyboardEvent event;
    event.windowsKeyCode = key;
    test_delegate_.Reset();
    external_delegate_->keyboard_listener()->HandleKeyPressEvent(event);
    test_delegate_.Wait();
  }
