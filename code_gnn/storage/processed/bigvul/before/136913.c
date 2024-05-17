bool HTMLInputElement::IsInRequiredRadioButtonGroup() {
  DCHECK_EQ(type(), InputTypeNames::radio);
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    return scope->IsInRequiredGroup(this);
  return false;
}
