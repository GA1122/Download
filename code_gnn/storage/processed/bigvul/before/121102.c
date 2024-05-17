void BaseMultipleFieldsDateAndTimeInputType::setValue(const String& sanitizedValue, bool valueChanged, TextFieldEventBehavior eventBehavior)
{
    InputType::setValue(sanitizedValue, valueChanged, eventBehavior);
    if (valueChanged || (sanitizedValue.isEmpty() && m_dateTimeEditElement && m_dateTimeEditElement->anyEditableFieldsHaveValues())) {
        updateInnerTextValue();
        element()->setNeedsValidityCheck();
    }
}
