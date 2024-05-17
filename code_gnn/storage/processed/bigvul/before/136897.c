RadioButtonGroupScope* HTMLInputElement::GetRadioButtonGroupScope() const {
  if (type() != InputTypeNames::radio)
    return nullptr;
  if (HTMLFormElement* form_element = Form())
    return &form_element->GetRadioButtonGroupScope();
  if (isConnected())
    return &GetTreeScope().GetRadioButtonGroupScope();
  return nullptr;
}
