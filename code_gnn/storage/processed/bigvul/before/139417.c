static bool ExecuteDeleteWordForward(LocalFrame& frame,
                                     Event*,
                                     EditorCommandSource,
                                     const String&) {
  frame.GetEditor().DeleteWithDirection(DeleteDirection::kForward,
                                        TextGranularity::kWord, true, false);
  return true;
}
