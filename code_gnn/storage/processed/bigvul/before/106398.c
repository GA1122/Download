void AppendMouseEvent(const WebInputEvent& event,
                      std::vector<PP_InputEvent>* pp_events) {
  COMPILE_ASSERT(static_cast<int>(WebMouseEvent::ButtonNone) ==
                 static_cast<int>(PP_INPUTEVENT_MOUSEBUTTON_NONE),
                 MouseNone);
  COMPILE_ASSERT(static_cast<int>(WebMouseEvent::ButtonLeft) ==
                 static_cast<int>(PP_INPUTEVENT_MOUSEBUTTON_LEFT),
                 MouseLeft);
  COMPILE_ASSERT(static_cast<int>(WebMouseEvent::ButtonRight) ==
                 static_cast<int>(PP_INPUTEVENT_MOUSEBUTTON_RIGHT),
                 MouseRight);
  COMPILE_ASSERT(static_cast<int>(WebMouseEvent::ButtonMiddle) ==
                 static_cast<int>(PP_INPUTEVENT_MOUSEBUTTON_MIDDLE),
                 MouseMiddle);

  const WebMouseEvent& mouse_event =
      reinterpret_cast<const WebMouseEvent&>(event);
  PP_InputEvent result = GetPPEventWithCommonFieldsAndType(event);
  result.u.mouse.modifier = mouse_event.modifiers;
  result.u.mouse.button =
      static_cast<PP_InputEvent_MouseButton>(mouse_event.button);
  result.u.mouse.x = static_cast<float>(mouse_event.x);
  result.u.mouse.y = static_cast<float>(mouse_event.y);
  result.u.mouse.click_count = mouse_event.clickCount;
  pp_events->push_back(result);
}
