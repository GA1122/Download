void PasswordAutofillAgent::FillPasswordForm(
    const PasswordFormFillData& form_data) {
  DCHECK(form_data.has_renderer_ids);
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(
        GetPasswordManagerDriver().get()));
    logger->LogMessage(Logger::STRING_ON_FILL_PASSWORD_FORM_METHOD);
  }

  bool username_password_fields_not_set =
      form_data.username_field.unique_renderer_id ==
          FormFieldData::kNotSetFormControlRendererId &&
      form_data.password_field.unique_renderer_id ==
          FormFieldData::kNotSetFormControlRendererId;
  if (username_password_fields_not_set) {
    MaybeStoreFallbackData(form_data);
    return;
  }

  WebInputElement username_element, password_element;
  std::tie(username_element, password_element) =
      FindUsernamePasswordElements(form_data);
  bool is_single_username_fill = form_data.password_field.unique_renderer_id ==
                                 FormFieldData::kNotSetFormControlRendererId;
  WebElement main_element =
      is_single_username_fill ? username_element : password_element;
  if (main_element.IsNull()) {
    MaybeStoreFallbackData(form_data);
    LogFirstFillingResult(form_data, FillingResult::kNoPasswordElement);
    return;
  }

  StoreDataForFillOnAccountSelect(form_data, username_element,
                                  password_element);

  if (form_data.wait_for_username) {
    LogFirstFillingResult(form_data, FillingResult::kWaitForUsername);
    return;
  }

  FillUserNameAndPassword(username_element, password_element, form_data,
                          logger.get());
}
