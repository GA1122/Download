bool PasswordAutofillAgent::DidClearAutofillSelection(
    const WebFormControlElement& control_element) {
  const WebInputElement* element = ToWebInputElement(&control_element);
  if (!element)
    return false;

  WebInputElement username_element;
  WebInputElement password_element;
  PasswordInfo* password_info;

  if (!FindPasswordInfoForElement(*element, &username_element,
                                  &password_element, &password_info)) {
    return false;
  }

  ClearPreview(&username_element, &password_element);
  return true;
}
