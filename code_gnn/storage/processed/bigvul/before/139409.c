static bool ExecuteDeleteBackward(LocalFrame& frame,
                                  Event*,
                                  EditorCommandSource,
                                  const String&) {
  frame.GetEditor().DeleteWithDirection(
      DeleteDirection::kBackward, TextGranularity::kCharacter, false, true);
  return true;
}
