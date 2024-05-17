bool SendKeyEvent(const std::string type,
                  int key_value,
                  int key_code,
                  std::string key_name,
                  int modifiers,
                  aura::WindowTreeHost* host) {
  ui::EventType event_type = ui::ET_UNKNOWN;
  if (type == kKeyDown)
    event_type = ui::ET_KEY_PRESSED;
  else if (type == kKeyUp)
    event_type = ui::ET_KEY_RELEASED;
  if (event_type == ui::ET_UNKNOWN)
    return false;

  ui::KeyboardCode code = static_cast<ui::KeyboardCode>(key_code);

  if (code == ui::VKEY_UNKNOWN) {
    if (event_type == ui::ET_KEY_RELEASED) {
      ui::InputMethod* input_method = host->window()->GetProperty(
          aura::client::kRootWindowInputMethodKey);
      if (!input_method)
        return false;

      ui::TextInputClient* tic = input_method->GetTextInputClient();

      SendProcessKeyEvent(ui::ET_KEY_PRESSED, host);
      tic->InsertChar(static_cast<uint16>(key_value), ui::EF_NONE);
      SendProcessKeyEvent(ui::ET_KEY_RELEASED, host);
    }
  } else {
    if (event_type == ui::ET_KEY_RELEASED) {
      static int keys_seen = 0;
      if (code == ui::VKEY_BACK) {
        UMA_HISTOGRAM_CUSTOM_COUNTS(
            "VirtualKeyboard.KeystrokesBetweenBackspaces",
            keys_seen, 1, 1000, 50);
        keys_seen = 0;
      } else {
        ++keys_seen;
      }
    }

    ui::DomCode dom_code = ui::DomCode::NONE;
    if (!key_name.empty())
      dom_code = ui::KeycodeConverter::CodeStringToDomCode(key_name.c_str());
    if (dom_code == ui::DomCode::NONE)
      dom_code = ui::UsLayoutKeyboardCodeToDomCode(code);
    CHECK(dom_code != ui::DomCode::NONE);
    ui::KeyEvent event(
        event_type,
        code,
        dom_code,
        modifiers);
    ui::EventDispatchDetails details =
        host->event_processor()->OnEventFromSource(&event);
    CHECK(!details.dispatcher_destroyed);
  }
  return true;
}
