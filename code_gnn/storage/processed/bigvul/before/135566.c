bool Editor::InsertText(const String& text, KeyboardEvent* triggering_event) {
  return GetFrame().GetEventHandler().HandleTextInputEvent(text,
                                                           triggering_event);
}
