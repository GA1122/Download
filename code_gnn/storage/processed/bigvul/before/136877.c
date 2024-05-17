void HTMLInputElement::DidChangeForm() {
  TextControlElement::DidChangeForm();
  if (input_type_)
    AddToRadioButtonGroup();
}
