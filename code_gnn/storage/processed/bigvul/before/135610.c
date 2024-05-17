void Editor::WriteSelectionToPasteboard() {
  KURL url = GetFrame().GetDocument()->Url();
  String html = GetFrame().Selection().SelectedHTMLForClipboard();
  String plain_text = GetFrame().SelectedTextForClipboard();
  Pasteboard::GeneralPasteboard()->WriteHTML(html, url, plain_text,
                                             CanSmartCopyOrDelete());
}
