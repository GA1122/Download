bool PasswordAutofillAgent::PreviewSuggestion(
    const WebFormControlElement& control_element,
    const WebString& username,
    const WebString& password) {
  const WebInputElement* element = ToWebInputElement(&control_element);
  if (!element)
    return false;

  WebInputElement username_element;
  WebInputElement password_element;
  PasswordInfo* password_info;

  if (!FindPasswordInfoForElement(*element, &username_element,
                                  &password_element, &password_info) ||
      (!password_element.IsNull() && !IsElementEditable(password_element))) {
    return false;
  }

  if (IsUsernameAmendable(username_element,
                          element->IsPasswordFieldForAutofill())) {
    if (username_query_prefix_.empty())
      username_query_prefix_ = username_element.Value().Utf16();

    username_autofill_state_ = username_element.GetAutofillState();
    username_element.SetSuggestedValue(username);
    username_element.SetAutofillState(WebAutofillState::kPreviewed);
    form_util::PreviewSuggestion(username_element.SuggestedValue().Utf16(),
                                 username_query_prefix_, &username_element);
  }
  if (!password_element.IsNull()) {
    password_autofill_state_ = password_element.GetAutofillState();
    password_element.SetSuggestedValue(password);
    password_element.SetAutofillState(WebAutofillState::kPreviewed);
  }

  return true;
}
