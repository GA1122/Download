void PasswordAutofillAgent::ClearPreview(
    blink::WebInputElement* username,
    blink::WebInputElement* password) {
  if (!username->isNull() && !username->suggestedValue().isEmpty()) {
    username->setSuggestedValue(blink::WebString());
    username->setAutofilled(was_username_autofilled_);
    username->setSelectionRange(username_query_prefix_.length(),
                                username->value().length());
  }
  if (!password->suggestedValue().isEmpty()) {
      password->setSuggestedValue(blink::WebString());
      password->setAutofilled(was_password_autofilled_);
  }
}
