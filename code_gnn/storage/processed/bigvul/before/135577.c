void Editor::PasteAsPlainTextWithPasteboard(Pasteboard* pasteboard) {
  String text = pasteboard->PlainText();
  PasteAsPlainText(text, CanSmartReplaceWithPasteboard(pasteboard));
}
