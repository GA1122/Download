void HTMLInputElement::setValueFromRenderer(const String& value)
{
    ASSERT(!isFileUpload());

    m_suggestedValue = String();

    ASSERT(value == sanitizeValue(value) || sanitizeValue(value).isEmpty());

    m_valueIfDirty = value == "\n" ? emptyString() : value;

    setFormControlValueMatchesRenderer(true);
    m_wasModifiedByUser = true;

    if (!isTextField())
        dispatchInputEvent();
    notifyFormStateChanged();

    setNeedsValidityCheck();

    setAutofilled(false);
}
