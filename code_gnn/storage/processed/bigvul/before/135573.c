void Editor::Paste(EditorCommandSource source) {
  DCHECK(GetFrame().GetDocument());
  if (TryDHTMLPaste(kAllMimeTypes))
    return;   
  if (!CanPaste())
    return;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (source == kCommandFromMenuOrKeyBinding &&
      !GetFrame().Selection().SelectionHasFocus())
    return;

  GetSpellChecker().UpdateMarkersForWordsAffectedByEditing(false);
  ResourceFetcher* loader = GetFrame().GetDocument()->Fetcher();
  ResourceCacheValidationSuppressor validation_suppressor(loader);

  const PasteMode paste_mode = CanEditRichly() ? kAllMimeTypes : kPlainTextOnly;

  if (source == kCommandFromMenuOrKeyBinding) {
    DataTransfer* data_transfer =
        DataTransfer::Create(DataTransfer::kCopyAndPaste, kDataTransferReadable,
                             DataObject::CreateFromPasteboard(paste_mode));

    if (DispatchBeforeInputDataTransfer(FindEventTargetFromSelection(),
                                        InputEvent::InputType::kInsertFromPaste,
                                        data_transfer) !=
        DispatchEventResult::kNotCanceled)
      return;
    if (frame_->GetDocument()->GetFrame() != frame_)
      return;
  }

  if (paste_mode == kAllMimeTypes)
    PasteWithPasteboard(Pasteboard::GeneralPasteboard());
  else
    PasteAsPlainTextWithPasteboard(Pasteboard::GeneralPasteboard());
}
