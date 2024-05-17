BaseMultipleFieldsDateAndTimeInputType::~BaseMultipleFieldsDateAndTimeInputType()
{
    if (m_spinButtonElement)
        m_spinButtonElement->removeSpinButtonOwner();
    if (m_clearButton)
        m_clearButton->removeClearButtonOwner();
    if (m_dateTimeEditElement)
        m_dateTimeEditElement->removeEditControlOwner();
    if (m_pickerIndicatorElement)
        m_pickerIndicatorElement->removePickerIndicatorOwner();
}
