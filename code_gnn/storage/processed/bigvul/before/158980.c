pp::MouseInputEvent NormalizeMouseEvent(pp::Instance* instance,
                                        const pp::MouseInputEvent& event) {
  pp::MouseInputEvent normalized_event = event;
#if defined(OS_MACOSX)
  uint32_t modifiers = event.GetModifiers();
  if ((modifiers & PP_INPUTEVENT_MODIFIER_CONTROLKEY) &&
      event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_LEFT &&
      event.GetType() == PP_INPUTEVENT_TYPE_MOUSEDOWN) {
    uint32_t new_modifiers = modifiers & ~PP_INPUTEVENT_MODIFIER_CONTROLKEY;
    normalized_event = pp::MouseInputEvent(
        instance, PP_INPUTEVENT_TYPE_MOUSEDOWN, event.GetTimeStamp(),
        new_modifiers, PP_INPUTEVENT_MOUSEBUTTON_RIGHT, event.GetPosition(), 1,
        event.GetMovement());
  }
#endif
  return normalized_event;
}
