void InputMethodController::DocumentAttached(Document* document) {
  DCHECK(document);
  SetContext(document);
}
