void ChromePasswordManagerClient::GenerationAvailableForForm(
    const autofill::PasswordForm& form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_generation_driver_bindings_.GetCurrentTargetFrame(), form,
          BadMessageReason::CPMD_BAD_ORIGIN_GENERATION_AVAILABLE_FOR_FORM))
    return;
  password_manager_.GenerationAvailableForForm(form);
}
