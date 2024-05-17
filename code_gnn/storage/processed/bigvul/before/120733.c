void BaseMultipleFieldsDateAndTimeInputType::setValue(const String& sanitizedValue, bool valueChanged, TextFieldEventBehavior eventBehavior)
{
    InputType::setValue(sanitizedValue, valueChanged, eventBehavior);
    DateTimeEditElement* edit = dateTimeEditElement();
    if (valueChanged || (sanitizedValue.isEmpty() && edit && edit->anyEditableFieldsHaveValues())) {
        updateInnerTextValue();
        element()->setNeedsValidityCheck();
    }
}
