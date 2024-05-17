void BaseMultipleFieldsDateAndTimeInputType::focusAndSelectSpinButtonOwner()
{
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->focusIfNoFocus();
}
