void Editor::PerformDelete() {
  if (!CanDelete())
    return;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  AddToKillRing(SelectedRange());
  DeleteSelectionWithSmartDelete(
      CanSmartCopyOrDelete() ? DeleteMode::kSmart : DeleteMode::kSimple,
      InputEvent::InputType::kDeleteContentBackward);

  SetStartNewKillRingSequence(false);
}
