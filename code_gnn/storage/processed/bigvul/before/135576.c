void Editor::PasteAsPlainText(EditorCommandSource source) {
  if (TryDHTMLPaste(kPlainTextOnly))
    return;
  if (!CanPaste())
    return;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (source == kCommandFromMenuOrKeyBinding &&
      !GetFrame().Selection().SelectionHasFocus())
    return;

  GetSpellChecker().UpdateMarkersForWordsAffectedByEditing(false);
  PasteAsPlainTextWithPasteboard(Pasteboard::GeneralPasteboard());
}
