void ChromePasswordManagerClient::SameDocumentNavigation(
    const autofill::PasswordForm& password_form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_manager_driver_bindings_.GetCurrentTargetFrame(),
          password_form, BadMessageReason::CPMD_BAD_ORIGIN_IN_PAGE_NAVIGATION))
    return;
  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  GetPasswordManager()->OnPasswordFormSubmittedNoChecks(driver, password_form);
}
