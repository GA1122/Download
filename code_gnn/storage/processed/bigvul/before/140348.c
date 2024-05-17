void Editor::pasteAsPlainTextWithPasteboard(Pasteboard* pasteboard) {
  String text = pasteboard->plainText();
  pasteAsPlainText(text, canSmartReplaceWithPasteboard(pasteboard));
}
