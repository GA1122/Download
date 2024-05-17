bool HTMLFormControlElement::isValidationMessageVisible() const {
  if (!m_hasValidationMessage)
    return false;

  ValidationMessageClient* client = validationMessageClient();
  if (!client)
    return false;

  return client->isValidationMessageVisible(*this);
}
