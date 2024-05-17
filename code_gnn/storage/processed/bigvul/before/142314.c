void ChromePasswordManagerClient::PasswordFormsParsed(
    const std::vector<autofill::PasswordForm>& forms) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_manager_driver_bindings_.GetCurrentTargetFrame(), forms,
          BadMessageReason::CPMD_BAD_ORIGIN_FORMS_PARSED))
    return;
  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  GetPasswordManager()->OnPasswordFormsParsed(driver, forms);
}
