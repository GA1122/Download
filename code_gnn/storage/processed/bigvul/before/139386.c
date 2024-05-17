static bool EnabledInEditableText(LocalFrame& frame,
                                  Event* event,
                                  EditorCommandSource source) {
  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (source == kCommandFromMenuOrKeyBinding &&
      !frame.Selection().SelectionHasFocus())
    return false;
  return frame.GetEditor().SelectionForCommand(event).RootEditableElement();
}
