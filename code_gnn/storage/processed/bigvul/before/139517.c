static bool ExecuteToggleOverwrite(LocalFrame& frame,
                                   Event*,
                                   EditorCommandSource,
                                   const String&) {
  frame.GetEditor().ToggleOverwriteModeEnabled();
  return true;
}
