bool DateTimeFormatValidator::validateFormat(const String& format, const BaseMultipleFieldsDateAndTimeInputType& inputType)
{
    if (!DateTimeFormat::parse(format, *this))
        return false;
    return inputType.isValidFormat(m_hasYear, m_hasMonth, m_hasWeek, m_hasDay, m_hasAMPM, m_hasHour, m_hasMinute, m_hasSecond);
}
