bool HTMLInputElement::WillRespondToMouseClickEvents() {
  if (!IsDisabledFormControl())
    return true;

  return TextControlElement::WillRespondToMouseClickEvents();
}
