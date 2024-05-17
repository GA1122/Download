void BaseMultipleFieldsDateAndTimeInputType::showPickerIndicator()
{
    if (m_pickerIndicatorIsVisible)
        return;
    m_pickerIndicatorIsVisible = true;
    ASSERT(m_pickerIndicatorElement);
    m_pickerIndicatorElement->removeInlineStyleProperty(CSSPropertyDisplay);
}
