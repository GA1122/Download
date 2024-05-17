void Editor::writeSelectionToPasteboard() {
  KURL url = frame().document()->url();
  String html = frame().selection().selectedHTMLForClipboard();
  String plainText = frame().selectedTextForClipboard();
  Pasteboard::generalPasteboard()->writeHTML(html, url, plainText,
                                             canSmartCopyOrDelete());
}
