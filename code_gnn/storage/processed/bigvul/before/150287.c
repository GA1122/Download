void PasswordAutofillAgent::FillIntoFocusedField(
    bool is_password,
    const base::string16& credential) {
  if (focused_input_element_.IsNull())
    return;
  if (!is_password) {
    FillField(&focused_input_element_, credential);
  }
  if (!focused_input_element_.IsPasswordFieldForAutofill())
    return;
  FillPasswordFieldAndSave(&focused_input_element_, credential);
}
