void PasswordAutofillAgent::ProvisionallySavePassword(
    const WebFormElement& form,
    const WebInputElement& element,
    ProvisionallySaveRestriction restriction) {
  DCHECK(!form.IsNull() || !element.IsNull());

  SetLastUpdatedFormAndField(form, element);
  std::unique_ptr<PasswordForm> password_form;
  if (form.IsNull()) {
    password_form = GetPasswordFormFromUnownedInputElements();
  } else {
    password_form = GetPasswordFormFromWebForm(form);
  }
  if (!password_form)
    return;

  bool has_password = FormHasNonEmptyPasswordField(password_form->form_data);
  if (restriction == RESTRICTION_NON_EMPTY_PASSWORD && !has_password)
    return;

  if (!FrameCanAccessPasswordManager())
    return;

  if (has_password) {
    GetPasswordManagerDriver()->ShowManualFallbackForSaving(*password_form);
  } else {
    GetPasswordManagerDriver()->HideManualFallbackForSaving();
  }
  browser_has_form_to_process_ = true;
}
