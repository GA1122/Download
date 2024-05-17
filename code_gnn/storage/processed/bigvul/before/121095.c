void BaseMultipleFieldsDateAndTimeInputType::hidePickerIndicator()
{
    if (!m_pickerIndicatorIsVisible)
        return;
    m_pickerIndicatorIsVisible = false;
    ASSERT(m_pickerIndicatorElement);
    m_pickerIndicatorElement->setInlineStyleProperty(CSSPropertyDisplay, CSSValueNone);
}
