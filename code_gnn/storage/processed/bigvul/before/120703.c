BaseMultipleFieldsDateAndTimeInputType::BaseMultipleFieldsDateAndTimeInputType(HTMLInputElement* element)
    : BaseDateAndTimeInputType(element)
    , m_isDestroyingShadowSubtree(false)
    , m_pickerIndicatorIsVisible(false)
    , m_pickerIndicatorIsAlwaysVisible(false)
{
}
