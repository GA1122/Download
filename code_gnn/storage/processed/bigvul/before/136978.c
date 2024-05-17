unsigned HTMLInputElement::SizeOfRadioGroup() const {
  RadioButtonGroupScope* scope = GetRadioButtonGroupScope();
  if (!scope)
    return 0;
  return scope->GroupSizeFor(this);
}
