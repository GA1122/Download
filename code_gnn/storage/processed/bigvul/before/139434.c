static bool ExecuteInsertNewline(LocalFrame& frame,
                                 Event* event,
                                 EditorCommandSource,
                                 const String&) {
  LocalFrame* target_frame = blink::TargetFrame(frame, event);
  return target_frame->GetEventHandler().HandleTextInputEvent(
      "\n", event,
      target_frame->GetEditor().CanEditRichly() ? kTextEventInputKeyboard
                                                : kTextEventInputLineBreak);
}
