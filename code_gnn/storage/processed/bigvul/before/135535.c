void Editor::Copy(EditorCommandSource source) {
  if (TryDHTMLCopy())
    return;   
  if (!CanCopy())
    return;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (source == kCommandFromMenuOrKeyBinding &&
      !GetFrame().Selection().SelectionHasFocus())
    return;

  if (EnclosingTextControl(
          GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Start())) {
    Pasteboard::GeneralPasteboard()->WritePlainText(
        GetFrame().SelectedTextForClipboard(),
        CanSmartCopyOrDelete() ? Pasteboard::kCanSmartReplace
                               : Pasteboard::kCannotSmartReplace);
  } else {
    Document* document = GetFrame().GetDocument();
    if (HTMLImageElement* image_element =
            ImageElementFromImageDocument(document))
      WriteImageNodeToPasteboard(Pasteboard::GeneralPasteboard(), image_element,
                                 document->title());
    else
      WriteSelectionToPasteboard();
  }
}
