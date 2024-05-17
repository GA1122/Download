bool Textfield::HasSelection() const {
  return !GetSelectedRange().is_empty();
}
