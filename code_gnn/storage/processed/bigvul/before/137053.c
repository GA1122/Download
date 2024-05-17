String InputType::DefaultToolTip(const InputTypeView& input_type_view) const {
  if (GetElement().Form() && GetElement().Form()->NoValidate())
    return String();
  return ValidationMessage(input_type_view).first;
}
