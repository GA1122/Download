bool PasswordAutofillAgent::FindPasswordInfoForElement(
    const blink::WebInputElement& element,
    blink::WebInputElement* username_element,
    blink::WebInputElement* password_element,
    PasswordInfo** password_info) {
  DCHECK(username_element && password_element && password_info);
  username_element->reset();
  password_element->reset();
  if (!element.isPasswordField()) {
    *username_element = element;
  } else {
    WebInputToPasswordInfoMap::iterator iter =
        web_input_to_password_info_.find(element);
    if (iter != web_input_to_password_info_.end()) {
      *password_element = element;
      *password_info = &iter->second;
      return true;
    }
    PasswordToLoginMap::const_iterator password_iter =
        password_to_username_.find(element);
    if (password_iter == password_to_username_.end()) {
      if (web_input_to_password_info_.empty())
        return false;

      *password_element = element;
      *password_info = &web_input_to_password_info_.begin()->second;
      return true;
    }
    *username_element = password_iter->second;
    *password_element = element;
  }

  WebInputToPasswordInfoMap::iterator iter =
      web_input_to_password_info_.find(*username_element);

  if (iter == web_input_to_password_info_.end())
    return false;

  *password_info = &iter->second;
  if (password_element->isNull())
    *password_element = (*password_info)->password_field;

  return true;
}
