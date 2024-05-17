void BaseMultipleFieldsDateAndTimeInputType::showPickerIndicator()
{
    if (m_pickerIndicatorIsVisible)
        return;
    m_pickerIndicatorIsVisible = true;
    ASSERT(pickerIndicatorElement());
    pickerIndicatorElement()->removeInlineStyleProperty(CSSPropertyDisplay);
}
