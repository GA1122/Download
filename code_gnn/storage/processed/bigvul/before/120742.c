void BaseMultipleFieldsDateAndTimeInputType::spinButtonStepDown()
{
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->stepDown();
}
