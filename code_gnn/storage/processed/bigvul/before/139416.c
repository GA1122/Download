static bool ExecuteDeleteWordBackward(LocalFrame& frame,
                                      Event*,
                                      EditorCommandSource,
                                      const String&) {
  frame.GetEditor().DeleteWithDirection(DeleteDirection::kBackward,
                                        TextGranularity::kWord, true, false);
  return true;
}
