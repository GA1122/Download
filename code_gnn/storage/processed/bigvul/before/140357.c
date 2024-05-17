bool Editor::replaceSelectionAfterDraggingWithEvents(
    Element* dropTarget,
    DragData* dragData,
    DocumentFragment* fragment,
    Range* dropCaretRange,
    InsertMode insertMode,
    DragSourceType dragSourceType) {
  if (!dropTarget || !dropTarget->isConnected())
    return true;

  DataTransfer* dataTransfer =
      DataTransfer::create(DataTransfer::DragAndDrop, DataTransferReadable,
                           dragData->platformData());
  dataTransfer->setSourceOperation(dragData->draggingSourceOperationMask());
  const bool shouldInsert =
      dispatchBeforeInputDataTransfer(
          dropTarget, InputEvent::InputType::InsertFromDrop, dataTransfer) ==
      DispatchEventResult::NotCanceled;

  if (m_frame->document()->frame() != m_frame)
    return false;

  if (shouldInsert && dropTarget->isConnected())
    replaceSelectionAfterDragging(fragment, insertMode, dragSourceType);

  return true;
}
