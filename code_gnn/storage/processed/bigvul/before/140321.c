bool Editor::dispatchCPPEvent(const AtomicString& eventType,
                              DataTransferAccessPolicy policy,
                              PasteMode pasteMode) {
  Element* target = findEventTargetFromSelection();
  if (!target)
    return true;

  DataTransfer* dataTransfer =
      DataTransfer::create(DataTransfer::CopyAndPaste, policy,
                           policy == DataTransferWritable
                               ? DataObject::create()
                               : DataObject::createFromPasteboard(pasteMode));

  Event* evt = ClipboardEvent::create(eventType, true, true, dataTransfer);
  target->dispatchEvent(evt);
  bool noDefaultProcessing = evt->defaultPrevented();
  if (noDefaultProcessing && policy == DataTransferWritable)
    Pasteboard::generalPasteboard()->writeDataObject(
        dataTransfer->dataObject());

  dataTransfer->setAccessPolicy(DataTransferNumb);

  return !noDefaultProcessing;
}
