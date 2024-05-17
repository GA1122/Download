static bool ExecuteDeleteToEndOfLine(LocalFrame& frame,
                                     Event*,
                                     EditorCommandSource,
                                     const String&) {
  frame.GetEditor().DeleteWithDirection(
      DeleteDirection::kForward, TextGranularity::kLineBoundary, true, false);
  return true;
}
