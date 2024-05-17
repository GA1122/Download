bool BaseMultipleFieldsDateAndTimeInputType::shouldSpinButtonRespondToWheelEvents()
{
    if (!shouldSpinButtonRespondToMouseEvents())
        return false;
    return m_dateTimeEditElement && m_dateTimeEditElement->hasFocusedField();
}
