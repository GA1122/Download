void ChromePasswordManagerClient::PasswordFormsRendered(
    const std::vector<autofill::PasswordForm>& visible_forms,
    bool did_stop_loading) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_manager_driver_bindings_.GetCurrentTargetFrame(),
          visible_forms, BadMessageReason::CPMD_BAD_ORIGIN_FORMS_RENDERED))
    return;
  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  GetPasswordManager()->OnPasswordFormsRendered(driver, visible_forms,
                                                did_stop_loading);
}
