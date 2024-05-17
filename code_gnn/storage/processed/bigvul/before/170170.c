bool PasswordAutofillAgent::TextFieldDidEndEditing(
    const blink::WebInputElement& element) {
  WebInputToPasswordInfoMap::const_iterator iter =
      web_input_to_password_info_.find(element);
  if (iter == web_input_to_password_info_.end())
    return false;

  const PasswordInfo& password_info = iter->second;
  if (password_info.password_was_edited_last)
    return false;

  const PasswordFormFillData& fill_data = password_info.fill_data;

  if (!fill_data.wait_for_username)
    return false;

  blink::WebInputElement password = password_info.password_field;
  if (!IsElementEditable(password))
    return false;

  blink::WebInputElement username = element;   

  FillUserNameAndPassword(&username, &password, fill_data, true, false,
                          &nonscript_modified_values_,
                          base::Bind(&PasswordValueGatekeeper::RegisterElement,
                                     base::Unretained(&gatekeeper_)),
                          nullptr);
  return true;
}
