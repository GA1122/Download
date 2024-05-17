bool Editor::insertText(const String& text, KeyboardEvent* triggeringEvent) {
  return frame().eventHandler().handleTextInputEvent(text, triggeringEvent);
}
