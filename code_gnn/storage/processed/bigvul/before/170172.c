void PasswordAutofillAgent::WillSendSubmitEvent(
    const blink::WebFormElement& form) {
  std::unique_ptr<PasswordForm> password_form = CreatePasswordFormFromWebForm(
      form, &nonscript_modified_values_, &form_predictions_);
  ProvisionallySavePassword(std::move(password_form),
                            RESTRICTION_NON_EMPTY_PASSWORD);
}
