  void SendKeyEvent(base::char16 ch, int flags) {
    if (ch < 0x80) {
      ui::KeyboardCode code =
          ch == ' ' ? ui::VKEY_SPACE :
          static_cast<ui::KeyboardCode>(ui::VKEY_A + ch - 'a');
      SendKeyPress(code, flags);
    } else {
      ui::KeyEvent event(ch, ui::VKEY_UNKNOWN, flags);
#if defined(OS_MACOSX)
      event_generator_->Dispatch(&event);
#else
      input_method_->DispatchKeyEvent(&event);
#endif
    }
  }
