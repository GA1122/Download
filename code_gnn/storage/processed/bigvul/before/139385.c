static bool EnabledDelete(LocalFrame& frame,
                          Event* event,
                          EditorCommandSource source) {
  switch (source) {
    case kCommandFromMenuOrKeyBinding:
      return frame.Selection().SelectionHasFocus() &&
             frame.GetEditor().CanDelete();
    case kCommandFromDOM:
      return EnabledInEditableText(frame, event, source);
  }
  NOTREACHED();
  return false;
}
