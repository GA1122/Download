void Editor::paste(EditorCommandSource source) {
  DCHECK(frame().document());
  if (tryDHTMLPaste(AllMimeTypes))
    return;   
  if (!canPaste())
    return;
  spellChecker().updateMarkersForWordsAffectedByEditing(false);
  ResourceFetcher* loader = frame().document()->fetcher();
  ResourceCacheValidationSuppressor validationSuppressor(loader);

  PasteMode pasteMode = frame().selection()
                                .computeVisibleSelectionInDOMTreeDeprecated()
                                .isContentRichlyEditable()
                            ? AllMimeTypes
                            : PlainTextOnly;

  if (source == CommandFromMenuOrKeyBinding) {
    DataTransfer* dataTransfer =
        DataTransfer::create(DataTransfer::CopyAndPaste, DataTransferReadable,
                             DataObject::createFromPasteboard(pasteMode));

    if (dispatchBeforeInputDataTransfer(findEventTargetFromSelection(),
                                        InputEvent::InputType::InsertFromPaste,
                                        dataTransfer) !=
        DispatchEventResult::NotCanceled)
      return;
    if (m_frame->document()->frame() != m_frame)
      return;
  }

  if (pasteMode == AllMimeTypes)
    pasteWithPasteboard(Pasteboard::generalPasteboard());
  else
    pasteAsPlainTextWithPasteboard(Pasteboard::generalPasteboard());
}
