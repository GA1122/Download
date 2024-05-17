void Editor::Cut(EditorCommandSource source) {
  if (TryDHTMLCut())
    return;   
  if (!CanCut())
    return;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (source == kCommandFromMenuOrKeyBinding &&
      !GetFrame().Selection().SelectionHasFocus())
    return;

  if (CanDeleteRange(SelectedRange())) {
    GetSpellChecker().UpdateMarkersForWordsAffectedByEditing(true);
    if (EnclosingTextControl(GetFrame()
                                 .Selection()
                                 .ComputeVisibleSelectionInDOMTree()
                                 .Start())) {
      String plain_text = GetFrame().SelectedTextForClipboard();
      Pasteboard::GeneralPasteboard()->WritePlainText(
          plain_text, CanSmartCopyOrDelete() ? Pasteboard::kCanSmartReplace
                                             : Pasteboard::kCannotSmartReplace);
    } else {
      WriteSelectionToPasteboard();
    }

    if (source == kCommandFromMenuOrKeyBinding) {
      if (DispatchBeforeInputDataTransfer(FindEventTargetFromSelection(),
                                          InputEvent::InputType::kDeleteByCut,
                                          nullptr) !=
          DispatchEventResult::kNotCanceled)
        return;
      if (frame_->GetDocument()->GetFrame() != frame_)
        return;
    }
    DeleteSelectionWithSmartDelete(
        CanSmartCopyOrDelete() ? DeleteMode::kSmart : DeleteMode::kSimple,
        InputEvent::InputType::kDeleteByCut);
  }
}
