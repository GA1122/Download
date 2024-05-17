static bool ExecuteMovePageDown(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  unsigned distance = VerticalScrollDistance(frame);
  if (!distance)
    return false;
  return ModifySelectionyWithPageGranularity(
      frame, SelectionModifyAlteration::kMove, distance,
      SelectionModifyVerticalDirection::kDown);
}
