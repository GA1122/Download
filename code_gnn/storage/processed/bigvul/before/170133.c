bool PasswordAutofillAgent::FillSuggestion(
    const blink::WebFormControlElement& control_element,
    const blink::WebString& username,
    const blink::WebString& password) {
  const blink::WebInputElement* element = toWebInputElement(&control_element);
  if (!element)
    return false;

  blink::WebInputElement username_element;
  blink::WebInputElement password_element;
  PasswordInfo* password_info;

  if (!FindPasswordInfoForElement(*element, &username_element,
                                  &password_element, &password_info) ||
      (!username_element.isNull() &&
       !IsElementAutocompletable(username_element)) ||
      !IsElementAutocompletable(password_element)) {
    return false;
  }

  password_info->password_was_edited_last = false;
  if (element->isPasswordField()) {
    password_info->password_field_suggestion_was_accepted = true;
    password_info->password_field = password_element;
  } else if (!username_element.isNull()) {
    username_element.setValue(username, true);
    username_element.setAutofilled(true);
    nonscript_modified_values_[username_element] = username;
  }

  password_element.setValue(password, true);
  password_element.setAutofilled(true);
  nonscript_modified_values_[password_element] = password;

  blink::WebInputElement mutable_filled_element = *element;
  mutable_filled_element.setSelectionRange(element->value().length(),
                                           element->value().length());

  return true;
}
