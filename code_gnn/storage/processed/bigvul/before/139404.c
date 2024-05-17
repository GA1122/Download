static bool ExecuteCopy(LocalFrame& frame,
                        Event*,
                        EditorCommandSource source,
                        const String&) {
  if (!CanWriteClipboard(frame, source))
    return false;
  frame.GetEditor().Copy(source);
  return true;
}
