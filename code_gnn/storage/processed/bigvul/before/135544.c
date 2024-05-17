void Editor::DeleteSelectionWithSmartDelete(
    DeleteMode delete_mode,
    InputEvent::InputType input_type,
    const Position& reference_move_position) {
  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone())
    return;

  const bool kMergeBlocksAfterDelete = true;
  const bool kExpandForSpecialElements = false;
  const bool kSanitizeMarkup = true;
  DCHECK(GetFrame().GetDocument());
  DeleteSelectionCommand::Create(
      *GetFrame().GetDocument(), delete_mode == DeleteMode::kSmart,
      kMergeBlocksAfterDelete, kExpandForSpecialElements, kSanitizeMarkup,
      input_type, reference_move_position)
      ->Apply();
}
