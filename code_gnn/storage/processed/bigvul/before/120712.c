void BaseMultipleFieldsDateAndTimeInputType::disabledAttributeChanged()
{
    spinButtonElement()->releaseCapture();
    clearButtonElement()->releaseCapture();
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->disabledStateChanged();
}
