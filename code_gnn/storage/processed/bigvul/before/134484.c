int ConvertAuraEventFlagsToWebInputEventModifiers(int aura_event_flags) {
  int web_input_event_modifiers = 0;
  if (aura_event_flags & ui::EF_SHIFT_DOWN)
    web_input_event_modifiers |= blink::WebInputEvent::ShiftKey;
  if (aura_event_flags & ui::EF_CONTROL_DOWN)
    web_input_event_modifiers |= blink::WebInputEvent::ControlKey;
  if (aura_event_flags & ui::EF_ALT_DOWN)
    web_input_event_modifiers |= blink::WebInputEvent::AltKey;
  if (aura_event_flags & ui::EF_COMMAND_DOWN)
    web_input_event_modifiers |= blink::WebInputEvent::MetaKey;
  return web_input_event_modifiers;
}
