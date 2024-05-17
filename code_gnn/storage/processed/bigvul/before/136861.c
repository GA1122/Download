HTMLInputElement* HTMLInputElement::CheckedRadioButtonForGroup() {
  if (checked())
    return this;
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    return scope->CheckedButtonForGroup(GetName());
  return nullptr;
}
