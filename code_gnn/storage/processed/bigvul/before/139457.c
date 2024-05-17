static bool ExecuteMovePageDownAndModifySelection(LocalFrame& frame,
                                                  Event*,
                                                  EditorCommandSource,
                                                  const String&) {
  unsigned distance = VerticalScrollDistance(frame);
  if (!distance)
    return false;
  return ModifySelectionyWithPageGranularity(
      frame, SelectionModifyAlteration::kExtend, distance,
      SelectionModifyVerticalDirection::kDown);
}
