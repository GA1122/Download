void Editor::copy() {
  if (tryDHTMLCopy())
    return;   
  if (!canCopy())
    return;

  frame().document()->updateStyleAndLayoutIgnorePendingStylesheets();

  if (enclosingTextControl(frame()
                               .selection()
                               .computeVisibleSelectionInDOMTreeDeprecated()
                               .start())) {
    Pasteboard::generalPasteboard()->writePlainText(
        frame().selectedTextForClipboard(),
        canSmartCopyOrDelete() ? Pasteboard::CanSmartReplace
                               : Pasteboard::CannotSmartReplace);
  } else {
    Document* document = frame().document();
    if (HTMLImageElement* imageElement =
            imageElementFromImageDocument(document))
      writeImageNodeToPasteboard(Pasteboard::generalPasteboard(), imageElement,
                                 document->title());
    else
      writeSelectionToPasteboard();
  }
}
