inline void HTMLInputElement::AddToRadioButtonGroup() {
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    scope->AddButton(this);
}
