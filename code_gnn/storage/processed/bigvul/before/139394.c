static bool EnabledVisibleSelection(LocalFrame& frame,
                                    Event* event,
                                    EditorCommandSource source) {
  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (source == kCommandFromMenuOrKeyBinding &&
      !frame.Selection().SelectionHasFocus())
    return false;

  const VisibleSelection& selection =
      frame.GetEditor().SelectionForCommand(event);
  return (selection.IsCaret() && selection.IsContentEditable()) ||
         selection.IsRange();
}
