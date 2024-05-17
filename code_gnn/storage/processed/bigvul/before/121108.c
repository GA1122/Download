void BaseMultipleFieldsDateAndTimeInputType::updateInnerTextValue()
{
    if (!m_dateTimeEditElement)
        return;

    DateTimeEditElement::LayoutParameters layoutParameters(element()->locale(), createStepRange(AnyIsDefaultStep));

    DateComponents date;
    const bool hasValue = parseToDateComponents(element()->value(), &date);
    if (!hasValue)
        setMillisecondToDateComponents(layoutParameters.stepRange.minimum().toDouble(), &date);

    setupLayoutParameters(layoutParameters, date);

    const AtomicString pattern = m_dateTimeEditElement->fastGetAttribute(HTMLNames::patternAttr);
    if (!pattern.isEmpty())
        layoutParameters.dateTimeFormat = pattern;

    if (!DateTimeFormatValidator().validateFormat(layoutParameters.dateTimeFormat, *this))
        layoutParameters.dateTimeFormat = layoutParameters.fallbackDateTimeFormat;

    if (hasValue)
        m_dateTimeEditElement->setValueAsDate(layoutParameters, date);
    else
        m_dateTimeEditElement->setEmptyValue(layoutParameters, date);
    updateClearButtonVisibility();
}
