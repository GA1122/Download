void BaseMultipleFieldsDateAndTimeInputType::spinButtonStepUp()
{
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->stepUp();
}
