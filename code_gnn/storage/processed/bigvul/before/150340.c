void PasswordAutofillAgent::StoreDataForFillOnAccountSelect(
    const PasswordFormFillData& form_data,
    WebInputElement username_element,
    WebInputElement password_element) {
  WebInputElement main_element =
      username_element.IsNull() ? password_element : username_element;

  PasswordInfo password_info;
  password_info.fill_data = form_data;
  password_info.password_field = password_element;
  web_input_to_password_info_[main_element] = password_info;
  last_supplied_password_info_iter_ =
      web_input_to_password_info_.find(main_element);
  if (!main_element.IsPasswordFieldForAutofill())
    password_to_username_[password_element] = username_element;
}
