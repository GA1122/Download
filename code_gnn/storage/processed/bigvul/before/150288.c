void PasswordAutofillAgent::FillPasswordFieldAndSave(
    WebInputElement* password_input,
    const base::string16& credential) {
  DCHECK(password_input);
  DCHECK(password_input->IsPasswordFieldForAutofill());
  FillField(password_input, credential);
  ProvisionallySavePassword(password_input->Form(), *password_input,
                            RESTRICTION_NONE);
}
