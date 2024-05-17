void HTMLInputElement::DisabledAttributeChanged() {
  TextControlElement::DisabledAttributeChanged();
  input_type_view_->DisabledAttributeChanged();
}
