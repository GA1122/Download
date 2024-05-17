void ChromePasswordManagerClient::ShowPasswordGenerationPopup(
    password_manager::ContentPasswordManagerDriver* driver,
    const autofill::password_generation::PasswordGenerationUIData& ui_data,
    bool is_manually_triggered) {
  DCHECK(driver);
  gfx::RectF element_bounds_in_top_frame_space =
      TransformToRootCoordinates(driver->render_frame_host(), ui_data.bounds);
  if (!is_manually_triggered &&
      driver->GetPasswordAutofillManager()
          ->MaybeShowPasswordSuggestionsWithGeneration(
              element_bounds_in_top_frame_space, ui_data.text_direction)) {
    return;
  }

  gfx::RectF element_bounds_in_screen_space =
      GetBoundsInScreenSpace(element_bounds_in_top_frame_space);
  password_manager_.SetGenerationElementAndReasonForForm(
      driver, ui_data.password_form, ui_data.generation_element,
      is_manually_triggered);

  popup_controller_ = PasswordGenerationPopupControllerImpl::GetOrCreate(
      popup_controller_, element_bounds_in_screen_space, ui_data.password_form,
      ui_data.generation_element, ui_data.max_length, &password_manager_,
      driver->AsWeakPtr(), observer_, web_contents(),
      web_contents()->GetNativeView());
  popup_controller_->Show(PasswordGenerationPopupController::kOfferGeneration);
}
