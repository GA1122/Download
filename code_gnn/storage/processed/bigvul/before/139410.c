static bool ExecuteDeleteBackwardByDecomposingPreviousCharacter(
    LocalFrame& frame,
    Event*,
    EditorCommandSource,
    const String&) {
  DLOG(ERROR) << "DeleteBackwardByDecomposingPreviousCharacter is not "
                 "implemented, doing DeleteBackward instead";
  frame.GetEditor().DeleteWithDirection(
      DeleteDirection::kBackward, TextGranularity::kCharacter, false, true);
  return true;
}
