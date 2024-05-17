static bool ExecutePaste(LocalFrame& frame,
                         Event*,
                         EditorCommandSource source,
                         const String&) {
  if (!CanReadClipboard(frame, source))
    return false;
  frame.GetEditor().Paste(source);
  return true;
}
