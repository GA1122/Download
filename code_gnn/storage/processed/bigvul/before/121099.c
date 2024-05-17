void BaseMultipleFieldsDateAndTimeInputType::requiredAttributeChanged()
{
    m_clearButton->releaseCapture();
    updateClearButtonVisibility();
}
