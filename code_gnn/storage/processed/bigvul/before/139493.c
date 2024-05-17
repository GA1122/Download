static bool ExecutePasteAndMatchStyle(LocalFrame& frame,
                                      Event*,
                                      EditorCommandSource source,
                                      const String&) {
  frame.GetEditor().PasteAsPlainText(source);
  return true;
}
