void ChromePasswordManagerClient::PresaveGeneratedPassword(
    const autofill::PasswordForm& password_form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_generation_driver_bindings_.GetCurrentTargetFrame(),
          password_form,
          BadMessageReason::CPMD_BAD_ORIGIN_PRESAVE_GENERATED_PASSWORD))
    return;
  if (popup_controller_)
    popup_controller_->UpdatePassword(password_form.password_value);

  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  password_manager_.OnPresaveGeneratedPassword(driver, password_form);
}
