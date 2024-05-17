bool BaseMultipleFieldsDateAndTimeInputType::hasBadInput() const
{
    DateTimeEditElement* edit = dateTimeEditElement();
    return element()->value().isEmpty() && edit && edit->anyEditableFieldsHaveValues();
}
