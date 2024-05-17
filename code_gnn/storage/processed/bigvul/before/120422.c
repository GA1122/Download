  void SendKeyToPopupAndWait(ui::KeyboardCode key) {
    content::NativeWebKeyboardEvent event;
    event.windowsKeyCode = key;
    event.type = blink::WebKeyboardEvent::RawKeyDown;
    test_delegate_.Reset();
    GetRenderViewHost()->AddKeyPressEventCallback(key_press_event_sink_);
    GetRenderViewHost()->ForwardKeyboardEvent(event);
    test_delegate_.Wait();
    GetRenderViewHost()->RemoveKeyPressEventCallback(key_press_event_sink_);
  }
