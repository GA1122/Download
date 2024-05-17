void BaseMultipleFieldsDateAndTimeInputType::readonlyAttributeChanged()
{
    spinButtonElement()->releaseCapture();
    clearButtonElement()->releaseCapture();
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->readOnlyStateChanged();
}
