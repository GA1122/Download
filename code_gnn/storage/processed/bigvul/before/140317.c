void Editor::cut(EditorCommandSource source) {
  if (tryDHTMLCut())
    return;   
  if (!canCut())
    return;

  frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();

  if (canDeleteRange(selectedRange())) {
    spellChecker().updateMarkersForWordsAffectedByEditing(true);
    if (enclosingTextControl(frame()
                                 .selection()
                                 .computeVisibleSelectionInDOMTreeDeprecated()
                                 .start())) {
      String plainText = frame().selectedTextForClipboard();
      Pasteboard::generalPasteboard()->writePlainText(
          plainText, canSmartCopyOrDelete() ? Pasteboard::CanSmartReplace
                                            : Pasteboard::CannotSmartReplace);
    } else {
      writeSelectionToPasteboard();
    }

    if (source == CommandFromMenuOrKeyBinding) {
      if (dispatchBeforeInputDataTransfer(findEventTargetFromSelection(),
                                          InputEvent::InputType::DeleteByCut,
                                          nullptr) !=
          DispatchEventResult::NotCanceled)
        return;
      if (m_frame->document()->frame() != m_frame)
        return;
    }
    deleteSelectionWithSmartDelete(
        canSmartCopyOrDelete() ? DeleteMode::Smart : DeleteMode::Simple,
        InputEvent::InputType::DeleteByCut);
  }
}
