void HTMLInputElement::WillChangeForm() {
  if (input_type_)
    RemoveFromRadioButtonGroup();
  TextControlElement::WillChangeForm();
}
