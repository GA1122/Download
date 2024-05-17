bool DevToolsEventForwarder::ForwardEvent(
    const content::NativeWebKeyboardEvent& event) {
  std::string event_type;
  switch (event.type) {
    case WebInputEvent::KeyDown:
    case WebInputEvent::RawKeyDown:
      event_type = kKeyDownEventName;
      break;
    case WebInputEvent::KeyUp:
      event_type = kKeyUpEventName;
      break;
    default:
      return false;
  }

  int key_code = ui::LocatedToNonLocatedKeyboardCode(
      static_cast<ui::KeyboardCode>(event.windowsKeyCode));
  int modifiers = event.modifiers & (WebInputEvent::ShiftKey |
                                     WebInputEvent::ControlKey |
                                     WebInputEvent::AltKey |
                                     WebInputEvent::MetaKey);
  int key = CombineKeyCodeAndModifiers(key_code, modifiers);
  if (whitelisted_keys_.find(key) == whitelisted_keys_.end())
    return false;

  base::DictionaryValue event_data;
  event_data.SetString("type", event_type);
  event_data.SetString("key", ui::KeycodeConverter::DomKeyToKeyString(
                                  static_cast<ui::DomKey>(event.domKey)));
  event_data.SetString("code", ui::KeycodeConverter::DomCodeToCodeString(
                                   static_cast<ui::DomCode>(event.domCode)));
  event_data.SetInteger("keyCode", key_code);
  event_data.SetInteger("modifiers", modifiers);
  devtools_window_->bindings_->CallClientFunction(
      "DevToolsAPI.keyEventUnhandled", &event_data, NULL, NULL);
  return true;
}
