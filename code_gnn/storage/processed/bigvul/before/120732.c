FormControlState BaseMultipleFieldsDateAndTimeInputType::saveFormControlState() const
{
    if (DateTimeEditElement* edit = dateTimeEditElement())
        return edit->valueAsDateTimeFieldsState().saveFormControlState();
    return FormControlState();
}
