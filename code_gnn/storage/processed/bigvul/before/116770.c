int RenderViewTest::SendKeyEvent(MockKeyboard::Layout layout,
                                 int key_code,
                                 MockKeyboard::Modifiers modifiers,
                                 string16* output) {
#if defined(OS_WIN)
  CHECK(mock_keyboard_.get());
  CHECK(output);
  int length = mock_keyboard_->GetCharacters(layout, key_code, modifiers,
                                             output);
  if (length != 1)
    return -1;

  MSG msg1 = { NULL, WM_KEYDOWN, key_code, 0 };
#if defined(USE_AURA)
  aura::KeyEvent evt1(msg1, false);
  NativeWebKeyboardEvent keydown_event(&evt1);
#else
  NativeWebKeyboardEvent keydown_event(msg1);
#endif
  SendNativeKeyEvent(keydown_event);

  MSG msg2 = { NULL, WM_CHAR, (*output)[0], 0 };
#if defined(USE_AURA)
  aura::KeyEvent evt2(msg2, true);
  NativeWebKeyboardEvent char_event(&evt2);
#else
  NativeWebKeyboardEvent char_event(msg2);
#endif
  SendNativeKeyEvent(char_event);

  MSG msg3 = { NULL, WM_KEYUP, key_code, 0 };
#if defined(USE_AURA)
  aura::KeyEvent evt3(msg3, false);
  NativeWebKeyboardEvent keyup_event(&evt3);
#else
  NativeWebKeyboardEvent keyup_event(msg3);
#endif
  SendNativeKeyEvent(keyup_event);

  return length;
#elif defined(USE_AURA) && defined(USE_X11)
  CHECK(output);
  const int flags = ConvertMockKeyboardModifier(modifiers);

  XEvent xevent1;
  InitXKeyEventForTesting(ui::ET_KEY_PRESSED,
                          static_cast<ui::KeyboardCode>(key_code),
                          flags,
                          &xevent1);
  aura::KeyEvent event1(&xevent1, false);
  NativeWebKeyboardEvent keydown_event(&event1);
  SendNativeKeyEvent(keydown_event);

  XEvent xevent2;
  InitXKeyEventForTesting(ui::ET_KEY_PRESSED,
                          static_cast<ui::KeyboardCode>(key_code),
                          flags,
                          &xevent2);
  aura::KeyEvent event2(&xevent2, true);
  NativeWebKeyboardEvent char_event(&event2);
  SendNativeKeyEvent(char_event);

  XEvent xevent3;
  InitXKeyEventForTesting(ui::ET_KEY_RELEASED,
                          static_cast<ui::KeyboardCode>(key_code),
                          flags,
                          &xevent3);
  aura::KeyEvent event3(&xevent3, false);
  NativeWebKeyboardEvent keyup_event(&event3);
  SendNativeKeyEvent(keyup_event);

  long c = GetCharacterFromKeyCode(static_cast<ui::KeyboardCode>(key_code),
                                   flags);
  output->assign(1, static_cast<char16>(c));
  return 1;
#elif defined(OS_LINUX)
  std::vector<GdkEvent*> events;
  ui::SynthesizeKeyPressEvents(
      NULL, static_cast<ui::KeyboardCode>(key_code),
      modifiers & (MockKeyboard::LEFT_CONTROL | MockKeyboard::RIGHT_CONTROL),
      modifiers & (MockKeyboard::LEFT_SHIFT | MockKeyboard::RIGHT_SHIFT),
      modifiers & (MockKeyboard::LEFT_ALT | MockKeyboard::RIGHT_ALT),
      &events);

  guint32 unicode_key = 0;
  for (size_t i = 0; i < events.size(); ++i) {
    if ((i + 1) == (events.size() / 2) || i == (events.size() / 2)) {
      unicode_key = gdk_keyval_to_unicode(events[i]->key.keyval);
      NativeWebKeyboardEvent webkit_event(events[i]);
      SendNativeKeyEvent(webkit_event);

      if (webkit_event.type == WebKit::WebInputEvent::RawKeyDown) {
        NativeWebKeyboardEvent char_event = webkit_event;
        char_event.type = WebKit::WebInputEvent::Char;
        char_event.skip_in_browser = true;
        SendNativeKeyEvent(char_event);
      }
    }
    gdk_event_free(events[i]);
  }

  output->assign(1, static_cast<char16>(unicode_key));
  return 1;
#else
  NOTIMPLEMENTED();
  return L'\0';
#endif
}
