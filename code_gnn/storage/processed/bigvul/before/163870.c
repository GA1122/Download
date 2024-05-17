bool DevToolsEventForwarder::ForwardEvent(
    const content::NativeWebKeyboardEvent& event) {
  std::string event_type;
  switch (event.GetType()) {
    case WebInputEvent::kKeyDown:
    case WebInputEvent::kRawKeyDown:
      event_type = kKeyDownEventName;
      break;
    case WebInputEvent::kKeyUp:
      event_type = kKeyUpEventName;
      break;
    default:
      return false;
  }

  int key_code = ui::LocatedToNonLocatedKeyboardCode(
      static_cast<ui::KeyboardCode>(event.windows_key_code));
  int modifiers = event.GetModifiers() &
                  (WebInputEvent::kShiftKey | WebInputEvent::kControlKey |
                   WebInputEvent::kAltKey | WebInputEvent::kMetaKey);
  int key = CombineKeyCodeAndModifiers(key_code, modifiers);
  if (whitelisted_keys_.find(key) == whitelisted_keys_.end())
    return false;

  base::DictionaryValue event_data;
  event_data.SetString("type", event_type);
  event_data.SetString("key", ui::KeycodeConverter::DomKeyToKeyString(
                                  static_cast<ui::DomKey>(event.dom_key)));
  event_data.SetString("code", ui::KeycodeConverter::DomCodeToCodeString(
                                   static_cast<ui::DomCode>(event.dom_code)));
  event_data.SetInteger("keyCode", key_code);
  event_data.SetInteger("modifiers", modifiers);
  devtools_window_->bindings_->CallClientFunction(
      "DevToolsAPI.keyEventUnhandled", &event_data, NULL, NULL);
  return true;
}
