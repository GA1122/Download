int GetEventModifiers(int modifiers,
                      bool auto_repeat,
                      bool is_keypad,
                      int location) {
  int result = blink::WebInputEvent::kFromDebugger;
  if (auto_repeat)
    result |= blink::WebInputEvent::kIsAutoRepeat;
  if (is_keypad)
    result |= blink::WebInputEvent::kIsKeyPad;

  if (modifiers & 1)
    result |= blink::WebInputEvent::kAltKey;
  if (modifiers & 2)
    result |= blink::WebInputEvent::kControlKey;
  if (modifiers & 4)
    result |= blink::WebInputEvent::kMetaKey;
  if (modifiers & 8)
    result |= blink::WebInputEvent::kShiftKey;

  if (location & 1)
    result |= blink::WebInputEvent::kIsLeft;
  if (location & 2)
    result |= blink::WebInputEvent::kIsRight;
  return result;
}
