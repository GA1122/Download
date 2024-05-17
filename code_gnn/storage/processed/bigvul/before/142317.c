void ChromePasswordManagerClient::PasswordNoLongerGenerated(
    const autofill::PasswordForm& password_form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_generation_driver_bindings_.GetCurrentTargetFrame(),
          password_form,
          BadMessageReason::CPMD_BAD_ORIGIN_PASSWORD_NO_LONGER_GENERATED))
    return;

  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  password_manager_.OnPasswordNoLongerGenerated(driver, password_form);

  PasswordGenerationPopupController* controller = popup_controller_.get();
  if (controller &&
      controller->state() ==
          PasswordGenerationPopupController::kEditGeneratedPassword) {
    HidePasswordGenerationPopup();
  }
}
