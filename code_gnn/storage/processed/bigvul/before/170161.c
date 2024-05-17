bool PasswordAutofillAgent::ProvisionallySavedPasswordIsValid() {
  return provisionally_saved_form_ &&
         !provisionally_saved_form_->username_value.empty() &&
         !(provisionally_saved_form_->password_value.empty() &&
         provisionally_saved_form_->new_password_value.empty());
}
