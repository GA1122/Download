static bool EnabledPaste(LocalFrame& frame,
                         Event*,
                         EditorCommandSource source) {
  if (!CanReadClipboard(frame, source))
    return false;
  if (source == kCommandFromMenuOrKeyBinding &&
      !frame.Selection().SelectionHasFocus())
    return false;
  return frame.GetEditor().CanPaste();
}
