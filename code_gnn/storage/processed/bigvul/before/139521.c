static bool ExecuteUndo(LocalFrame& frame,
                        Event*,
                        EditorCommandSource,
                        const String&) {
  frame.GetEditor().Undo();
  return true;
}
