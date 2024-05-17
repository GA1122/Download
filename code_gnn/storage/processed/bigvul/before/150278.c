void PasswordAutofillAgent::ClearPreview(WebInputElement* username,
                                         WebInputElement* password) {
  if (!username->IsNull() && !username->SuggestedValue().IsEmpty()) {
    username->SetSuggestedValue(WebString());
    username->SetAutofillState(username_autofill_state_);
    username->SetSelectionRange(username_query_prefix_.length(),
                                username->Value().length());
  }
  if (!password->IsNull() && !password->SuggestedValue().IsEmpty()) {
    password->SetSuggestedValue(WebString());
    password->SetAutofillState(password_autofill_state_);
  }
}
