void Editor::performDelete() {
  if (!canDelete())
    return;

  frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();

  addToKillRing(selectedRange());
  deleteSelectionWithSmartDelete(
      canSmartCopyOrDelete() ? DeleteMode::Smart : DeleteMode::Simple,
      InputEvent::InputType::DeleteContentBackward);

  setStartNewKillRingSequence(false);
}
