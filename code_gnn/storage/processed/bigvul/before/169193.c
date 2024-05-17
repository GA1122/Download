void BuildSimpleWebKeyEvent(blink::WebInputEvent::Type type,
                            ui::DomKey key,
                            ui::DomCode code,
                            ui::KeyboardCode key_code,
                            NativeWebKeyboardEvent* event) {
  event->dom_key = key;
  event->dom_code = static_cast<int>(code);
  event->native_key_code = ui::KeycodeConverter::DomCodeToNativeKeycode(code);
  event->windows_key_code = key_code;
  event->is_system_key = false;
  event->skip_in_browser = true;

  if (type == blink::WebInputEvent::kChar ||
      type == blink::WebInputEvent::kRawKeyDown) {
    event->text[0] = key_code;
    event->unmodified_text[0] = key_code;
  }
}
