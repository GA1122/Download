void DragController::MouseMovedIntoDocument(Document* new_document) {
  if (document_under_mouse_ == new_document)
    return;

  if (document_under_mouse_)
    CancelDrag();
  document_under_mouse_ = new_document;
}
