static bool ExecuteRedo(LocalFrame& frame,
                        Event*,
                        EditorCommandSource,
                        const String&) {
  frame.GetEditor().Redo();
  return true;
}
