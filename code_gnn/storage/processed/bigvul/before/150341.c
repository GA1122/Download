bool PasswordAutofillAgent::TextDidChangeInTextField(
    const WebInputElement& element) {
  WebInputElement mutable_element = element;   
  mutable_element.SetAutofillState(WebAutofillState::kNotFilled);

  auto iter = web_input_to_password_info_.find(element);
  if (iter != web_input_to_password_info_.end()) {
    iter->second.password_was_edited_last = false;
  }

  return ShowSuggestions(element, false, false);
}
