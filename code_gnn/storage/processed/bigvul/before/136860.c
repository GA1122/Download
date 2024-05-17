bool HTMLInputElement::CanStartSelection() const {
  if (!IsTextField())
    return false;
  return TextControlElement::CanStartSelection();
}
