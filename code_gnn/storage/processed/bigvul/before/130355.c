void HTMLFormControlElement::hideVisibleValidationMessage()
{
    if (!m_hasValidationMessage)
        return;

    if (ValidationMessageClient* client = validationMessageClient())
        client->hideValidationMessage(*this);
}
