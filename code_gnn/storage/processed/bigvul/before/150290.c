bool PasswordAutofillAgent::FillSuggestion(
    const WebFormControlElement& control_element,
    const base::string16& username,
    const base::string16& password) {
  const WebInputElement* element = ToWebInputElement(&control_element);
  if (!element)
    return false;

  WebInputElement username_element;
  WebInputElement password_element;
  PasswordInfo* password_info = nullptr;

  if (!FindPasswordInfoForElement(*element, &username_element,
                                  &password_element, &password_info) ||
      (!password_element.IsNull() && !IsElementEditable(password_element))) {
    return false;
  }

  password_info->password_was_edited_last = false;
  if (element->IsPasswordFieldForAutofill()) {
    password_info->password_field_suggestion_was_accepted = true;
    password_info->password_field = password_element;
  }

  if (!password_element.IsNull() && password_generation_agent_)
    password_generation_agent_->OnFieldAutofilled(password_element);

  if (IsUsernameAmendable(username_element,
                          element->IsPasswordFieldForAutofill()) &&
      username_element.Value().Utf16() != username) {
    FillField(&username_element, username);
  }

  if (!password_element.IsNull())
    FillPasswordFieldAndSave(&password_element, password);

  WebInputElement mutable_filled_element = *element;
  mutable_filled_element.SetSelectionRange(element->Value().length(),
                                           element->Value().length());

  return true;
}
