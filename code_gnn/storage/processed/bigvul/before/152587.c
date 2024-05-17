  int SendKeyEvent(MockKeyboard::Layout layout,
                   int key_code,
                   MockKeyboard::Modifiers modifiers,
                   base::string16* output) {
#if defined(OS_WIN)
    CHECK(mock_keyboard_.get());
    CHECK(output);
    int length = mock_keyboard_->GetCharacters(layout, key_code, modifiers,
                                               output);
    if (length != 1)
      return -1;

    MSG msg1 = { NULL, WM_KEYDOWN, key_code, 0 };
    ui::KeyEvent evt1(msg1);
    NativeWebKeyboardEvent keydown_event(evt1);
    SendNativeKeyEvent(keydown_event);

    MSG msg2 = { NULL, WM_CHAR, (*output)[0], 0 };
    ui::KeyEvent evt2(msg2);
    NativeWebKeyboardEvent char_event(evt2);
    SendNativeKeyEvent(char_event);

    MSG msg3 = { NULL, WM_KEYUP, key_code, 0 };
    ui::KeyEvent evt3(msg3);
    NativeWebKeyboardEvent keyup_event(evt3);
    SendNativeKeyEvent(keyup_event);

    return length;
#elif defined(USE_AURA) && defined(USE_X11)
    CHECK(output);
    const int flags = ConvertMockKeyboardModifier(modifiers);

    ui::ScopedXI2Event xevent;
    xevent.InitKeyEvent(ui::ET_KEY_PRESSED,
                        static_cast<ui::KeyboardCode>(key_code),
                        flags);
    ui::KeyEvent event1(xevent);
    NativeWebKeyboardEvent keydown_event(event1);
    SendNativeKeyEvent(keydown_event);

    xevent.InitKeyEvent(ui::ET_KEY_PRESSED,
                        static_cast<ui::KeyboardCode>(key_code),
                        flags);
    ui::KeyEvent event2(xevent);
    event2.set_character(
        DomCodeToUsLayoutCharacter(event2.code(), event2.flags()));
    ui::KeyEventTestApi test_event2(&event2);
    test_event2.set_is_char(true);
    NativeWebKeyboardEvent char_event(event2);
    SendNativeKeyEvent(char_event);

    xevent.InitKeyEvent(ui::ET_KEY_RELEASED,
                        static_cast<ui::KeyboardCode>(key_code),
                        flags);
    ui::KeyEvent event3(xevent);
    NativeWebKeyboardEvent keyup_event(event3);
    SendNativeKeyEvent(keyup_event);

    long c = DomCodeToUsLayoutCharacter(
        UsLayoutKeyboardCodeToDomCode(static_cast<ui::KeyboardCode>(key_code)),
        flags);
    output->assign(1, static_cast<base::char16>(c));
    return 1;
#elif defined(USE_OZONE)
    const int flags = ConvertMockKeyboardModifier(modifiers);

    ui::KeyEvent keydown_event(ui::ET_KEY_PRESSED,
                               static_cast<ui::KeyboardCode>(key_code),
                               flags);
    NativeWebKeyboardEvent keydown_web_event(keydown_event);
    SendNativeKeyEvent(keydown_web_event);

    ui::KeyEvent char_event(keydown_event.GetCharacter(),
                            static_cast<ui::KeyboardCode>(key_code),
                            ui::DomCode::NONE, flags);
    NativeWebKeyboardEvent char_web_event(char_event);
    SendNativeKeyEvent(char_web_event);

    ui::KeyEvent keyup_event(ui::ET_KEY_RELEASED,
                             static_cast<ui::KeyboardCode>(key_code),
                             flags);
    NativeWebKeyboardEvent keyup_web_event(keyup_event);
    SendNativeKeyEvent(keyup_web_event);

    long c = DomCodeToUsLayoutCharacter(
        UsLayoutKeyboardCodeToDomCode(static_cast<ui::KeyboardCode>(key_code)),
        flags);
    output->assign(1, static_cast<base::char16>(c));
    return 1;
#else
    NOTIMPLEMENTED();
    return L'\0';
#endif
  }
