void Editor::PasteWithPasteboard(Pasteboard* pasteboard) {
  DocumentFragment* fragment = nullptr;
  bool chose_plain_text = false;

  if (pasteboard->IsHTMLAvailable()) {
    unsigned fragment_start = 0;
    unsigned fragment_end = 0;
    KURL url;
    String markup = pasteboard->ReadHTML(url, fragment_start, fragment_end);
    if (!markup.IsEmpty()) {
      DCHECK(GetFrame().GetDocument());
      fragment = CreateFragmentFromMarkupWithContext(
          *GetFrame().GetDocument(), markup, fragment_start, fragment_end, url,
          kDisallowScriptingAndPluginContent);
    }
  }

  if (!fragment) {
    String text = pasteboard->PlainText();
    if (!text.IsEmpty()) {
      chose_plain_text = true;

      GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

      fragment = CreateFragmentFromText(SelectedRange(), text);
    }
  }

  if (fragment)
    PasteAsFragment(fragment, CanSmartReplaceWithPasteboard(pasteboard),
                    chose_plain_text);
}
