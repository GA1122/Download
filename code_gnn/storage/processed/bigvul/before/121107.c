void BaseMultipleFieldsDateAndTimeInputType::updateClearButtonVisibility()
{
    if (!m_clearButton)
        return;

    if (element()->isRequired() || !m_dateTimeEditElement->anyEditableFieldsHaveValues())
        m_clearButton->setInlineStyleProperty(CSSPropertyVisibility, CSSValueHidden);
    else
        m_clearButton->removeInlineStyleProperty(CSSPropertyVisibility);
}
