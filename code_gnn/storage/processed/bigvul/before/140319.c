void Editor::deleteSelectionWithSmartDelete(
    DeleteMode deleteMode,
    InputEvent::InputType inputType,
    const Position& referenceMovePosition) {
  if (frame().selection().computeVisibleSelectionInDOMTreeDeprecated().isNone())
    return;

  const bool kMergeBlocksAfterDelete = true;
  const bool kExpandForSpecialElements = false;
  const bool kSanitizeMarkup = true;
  DCHECK(frame().document());
  DeleteSelectionCommand::create(
      *frame().document(), deleteMode == DeleteMode::Smart,
      kMergeBlocksAfterDelete, kExpandForSpecialElements, kSanitizeMarkup,
      inputType, referenceMovePosition)
      ->apply();
}
