bool HTMLFormControlElement::ShouldHaveFocusAppearance() const {
  return (GetDocument().LastFocusType() != kWebFocusTypeMouse) ||
         GetDocument().HadKeyboardEvent() || MayTriggerVirtualKeyboard();
}
