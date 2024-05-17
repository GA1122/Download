void ChromePasswordManagerClient::PasswordFormSubmitted(
    const autofill::PasswordForm& password_form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_manager_driver_bindings_.GetCurrentTargetFrame(),
          password_form, BadMessageReason::CPMD_BAD_ORIGIN_FORM_SUBMITTED))
    return;
  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  GetPasswordManager()->OnPasswordFormSubmitted(driver, password_form);
}
