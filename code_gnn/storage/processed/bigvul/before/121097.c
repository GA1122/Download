void BaseMultipleFieldsDateAndTimeInputType::pickerIndicatorChooseValue(const String& value)
{
    if (element()->isValidValue(value)) {
        element()->setValue(value, DispatchInputAndChangeEvent);
        return;
    }

    if (!m_dateTimeEditElement)
        return;
    DateComponents date;
    unsigned end;
    if (date.parseDate(value.characters(), value.length(), 0, end) && end == value.length())
        m_dateTimeEditElement->setOnlyYearMonthDay(date);
}
