static bool EnabledCopy(LocalFrame& frame, Event*, EditorCommandSource source) {
  if (!CanWriteClipboard(frame, source))
    return false;
  if (source == kCommandFromMenuOrKeyBinding &&
      !frame.Selection().SelectionHasFocus())
    return false;
  return frame.GetEditor().CanDHTMLCopy() || frame.GetEditor().CanCopy();
}
