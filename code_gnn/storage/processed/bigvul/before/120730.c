void BaseMultipleFieldsDateAndTimeInputType::requiredAttributeChanged()
{
    clearButtonElement()->releaseCapture();
    updateClearButtonVisibility();
}
