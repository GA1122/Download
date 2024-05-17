BaseMultipleFieldsDateAndTimeInputType::BaseMultipleFieldsDateAndTimeInputType(HTMLInputElement* element)
    : BaseDateAndTimeInputType(element)
    , m_dateTimeEditElement(0)
    , m_spinButtonElement(0)
    , m_clearButton(0)
    , m_pickerIndicatorElement(0)
    , m_pickerIndicatorIsVisible(false)
    , m_pickerIndicatorIsAlwaysVisible(false)
{
}
