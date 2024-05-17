void BaseMultipleFieldsDateAndTimeInputType::updateInnerTextValue()
{
    DateTimeEditElement* edit = dateTimeEditElement();
    if (!edit)
        return;

    DateTimeEditElement::LayoutParameters layoutParameters(element()->locale(), createStepRange(AnyIsDefaultStep));

    DateComponents date;
    const bool hasValue = parseToDateComponents(element()->value(), &date);
    if (!hasValue)
        setMillisecondToDateComponents(layoutParameters.stepRange.minimum().toDouble(), &date);

    setupLayoutParameters(layoutParameters, date);

    const AtomicString pattern = edit->fastGetAttribute(HTMLNames::patternAttr);
    if (!pattern.isEmpty())
        layoutParameters.dateTimeFormat = pattern;

    if (!DateTimeFormatValidator().validateFormat(layoutParameters.dateTimeFormat, *this))
        layoutParameters.dateTimeFormat = layoutParameters.fallbackDateTimeFormat;

    if (hasValue)
        edit->setValueAsDate(layoutParameters, date);
    else
        edit->setEmptyValue(layoutParameters, date);
    updateClearButtonVisibility();
}
