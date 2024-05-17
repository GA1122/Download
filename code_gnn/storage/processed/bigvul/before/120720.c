void BaseMultipleFieldsDateAndTimeInputType::hidePickerIndicator()
{
    if (!m_pickerIndicatorIsVisible)
        return;
    m_pickerIndicatorIsVisible = false;
    ASSERT(pickerIndicatorElement());
    pickerIndicatorElement()->setInlineStyleProperty(CSSPropertyDisplay, CSSValueNone);
}
