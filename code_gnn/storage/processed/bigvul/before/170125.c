bool PasswordAutofillAgent::DidClearAutofillSelection(
    const blink::WebFormControlElement& control_element) {
  const blink::WebInputElement* element = toWebInputElement(&control_element);
  if (!element)
    return false;

  blink::WebInputElement username_element;
  blink::WebInputElement password_element;
  PasswordInfo* password_info;

  if (!FindPasswordInfoForElement(*element, &username_element,
                                  &password_element, &password_info))
    return false;

  ClearPreview(&username_element, &password_element);
  return true;
}
