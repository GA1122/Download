Document& InputMethodController::GetDocument() const {
  DCHECK(IsAvailable());
  return *GetFrame().GetDocument();
}
