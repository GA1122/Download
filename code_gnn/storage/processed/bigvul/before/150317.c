void PasswordAutofillAgent::MaybeStoreFallbackData(
    const PasswordFormFillData& form_data) {
  if (!web_input_to_password_info_.empty())
    return;
  PasswordInfo password_info;
  password_info.fill_data = form_data;
  web_input_to_password_info_[WebInputElement()] = password_info;
  last_supplied_password_info_iter_ = web_input_to_password_info_.begin();
}
