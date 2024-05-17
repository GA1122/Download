void HTMLInputElement::RequiredAttributeChanged() {
  TextControlElement::RequiredAttributeChanged();
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    scope->RequiredAttributeChanged(this);
  input_type_view_->RequiredAttributeChanged();
}
