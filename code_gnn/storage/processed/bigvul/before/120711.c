DateTimeEditElement* BaseMultipleFieldsDateAndTimeInputType::dateTimeEditElement() const
{
    return toDateTimeEditElement(elementById(ShadowElementNames::dateTimeEdit()));
}
