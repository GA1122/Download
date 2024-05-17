void Editor::pasteWithPasteboard(Pasteboard* pasteboard) {
  DocumentFragment* fragment = nullptr;
  bool chosePlainText = false;

  if (pasteboard->isHTMLAvailable()) {
    unsigned fragmentStart = 0;
    unsigned fragmentEnd = 0;
    KURL url;
    String markup = pasteboard->readHTML(url, fragmentStart, fragmentEnd);
    if (!markup.isEmpty()) {
      DCHECK(frame().document());
      fragment = createFragmentFromMarkupWithContext(
          *frame().document(), markup, fragmentStart, fragmentEnd, url,
          DisallowScriptingAndPluginContent);
    }
  }

  if (!fragment) {
    String text = pasteboard->plainText();
    if (!text.isEmpty()) {
      chosePlainText = true;

      frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();

      fragment = createFragmentFromText(selectedRange(), text);
    }
  }

  if (fragment)
    pasteAsFragment(fragment, canSmartReplaceWithPasteboard(pasteboard),
                    chosePlainText);
}
