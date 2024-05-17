void BaseMultipleFieldsDateAndTimeInputType::destroyShadowSubtree()
{
    if (m_spinButtonElement) {
        m_spinButtonElement->removeSpinButtonOwner();
        m_spinButtonElement = 0;
    }
    if (m_clearButton) {
        m_clearButton->removeClearButtonOwner();
        m_clearButton = 0;
    }
    if (m_dateTimeEditElement) {
        m_dateTimeEditElement->removeEditControlOwner();
        m_dateTimeEditElement = 0;
    }
    if (m_pickerIndicatorElement) {
        m_pickerIndicatorElement->removePickerIndicatorOwner();
        m_pickerIndicatorElement = 0;
    }

    if (element()->focused())
        element()->focus();

    BaseDateAndTimeInputType::destroyShadowSubtree();
}
