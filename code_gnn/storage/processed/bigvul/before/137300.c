bool Textfield::ShouldBlinkCursor() const {
  return ShouldShowCursor() && !Textfield::GetCaretBlinkInterval().is_zero();
}
