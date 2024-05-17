static bool ExecuteRemoveFormat(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  frame.GetEditor().RemoveFormattingAndStyle();
  return true;
}
