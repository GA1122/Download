void BaseMultipleFieldsDateAndTimeInputType::blur()
{
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->blurByOwner();
}
