void ChromePasswordManagerClient::ShowPasswordSuggestions(
    base::i18n::TextDirection text_direction,
    const base::string16& typed_username,
    int options,
    const gfx::RectF& bounds) {
  password_manager::PasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(
          password_manager_driver_bindings_.GetCurrentTargetFrame());
  driver->GetPasswordAutofillManager()->OnShowPasswordSuggestions(
      text_direction, typed_username, options,
      TransformToRootCoordinates(
          password_manager_driver_bindings_.GetCurrentTargetFrame(), bounds));
}
