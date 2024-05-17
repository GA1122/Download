static bool ExecuteMovePageUp(LocalFrame& frame,
                              Event*,
                              EditorCommandSource,
                              const String&) {
  unsigned distance = VerticalScrollDistance(frame);
  if (!distance)
    return false;
  return ModifySelectionyWithPageGranularity(
      frame, SelectionModifyAlteration::kMove, distance,
      SelectionModifyVerticalDirection::kUp);
}
