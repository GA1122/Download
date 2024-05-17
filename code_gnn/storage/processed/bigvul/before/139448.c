static bool ExecuteMoveBackward(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  frame.Selection().Modify(SelectionModifyAlteration::kMove,
                           SelectionModifyDirection::kBackward,
                           TextGranularity::kCharacter, SetSelectionBy::kUser);
  return true;
}
