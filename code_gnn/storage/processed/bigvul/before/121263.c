void HTMLInputElement::setValue(const String& value, TextFieldEventBehavior eventBehavior)
{
    if (!m_inputType->canSetValue(value))
        return;

    RefPtr<HTMLInputElement> protector(this);
    EventQueueScope scope;
    String sanitizedValue = sanitizeValue(value);
    bool valueChanged = sanitizedValue != this->value();

    setLastChangeWasNotUserEdit();
    setFormControlValueMatchesRenderer(false);
    m_suggestedValue = String();  
    m_inputType->setValue(sanitizedValue, valueChanged, eventBehavior);

    if (!valueChanged)
        return;

    notifyFormStateChanged();
}
