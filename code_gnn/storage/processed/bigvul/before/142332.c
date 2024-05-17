void ChromePasswordManagerClient::ShowPasswordEditingPopup(
    const gfx::RectF& bounds,
    const autofill::PasswordForm& form) {
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          password_generation_driver_bindings_.GetCurrentTargetFrame(), form,
          BadMessageReason::CPMD_BAD_ORIGIN_SHOW_PASSWORD_EDITING_POPUP))
    return;
  auto* driver = driver_factory_->GetDriverForFrame(
      password_generation_driver_bindings_.GetCurrentTargetFrame());
  DCHECK(driver);
  gfx::RectF element_bounds_in_screen_space =
      GetBoundsInScreenSpace(TransformToRootCoordinates(
          password_manager_driver_bindings_.GetCurrentTargetFrame(), bounds));
  popup_controller_ = PasswordGenerationPopupControllerImpl::GetOrCreate(
      popup_controller_, element_bounds_in_screen_space, form,
      base::string16(),   
      0,                  
      &password_manager_, driver->AsWeakPtr(), observer_, web_contents(),
      web_contents()->GetNativeView());
  DCHECK(!form.password_value.empty());
  popup_controller_->UpdatePassword(form.password_value);
  popup_controller_->Show(
      PasswordGenerationPopupController::kEditGeneratedPassword);
}
