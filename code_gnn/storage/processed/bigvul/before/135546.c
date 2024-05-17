bool Editor::DispatchCPPEvent(const AtomicString& event_type,
                              DataTransferAccessPolicy policy,
                              PasteMode paste_mode) {
  Element* target = FindEventTargetFromSelection();
  if (!target)
    return true;

  DataTransfer* data_transfer =
      DataTransfer::Create(DataTransfer::kCopyAndPaste, policy,
                           policy == kDataTransferWritable
                               ? DataObject::Create()
                               : DataObject::CreateFromPasteboard(paste_mode));

  Event* evt = ClipboardEvent::Create(event_type, true, true, data_transfer);
  target->DispatchEvent(evt);
  bool no_default_processing = evt->defaultPrevented();
  if (no_default_processing && policy == kDataTransferWritable)
    Pasteboard::GeneralPasteboard()->WriteDataObject(
        data_transfer->GetDataObject());

  data_transfer->SetAccessPolicy(kDataTransferNumb);

  return !no_default_processing;
}
