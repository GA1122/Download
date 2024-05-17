void DateTimeFormatValidator::visitField(DateTimeFormat::FieldType fieldType, int)
{
    switch (fieldType) {
    case DateTimeFormat::FieldTypeYear:
        m_hasYear = true;
        break;
    case DateTimeFormat::FieldTypeMonth:  
    case DateTimeFormat::FieldTypeMonthStandAlone:
        m_hasMonth = true;
        break;
    case DateTimeFormat::FieldTypeWeekOfYear:
        m_hasWeek = true;
        break;
    case DateTimeFormat::FieldTypeDayOfMonth:
        m_hasDay = true;
        break;
    case DateTimeFormat::FieldTypePeriod:
        m_hasAMPM = true;
        break;
    case DateTimeFormat::FieldTypeHour11:  
    case DateTimeFormat::FieldTypeHour12:
        m_hasHour = true;
        break;
    case DateTimeFormat::FieldTypeHour23:  
    case DateTimeFormat::FieldTypeHour24:
        m_hasHour = true;
        m_hasAMPM = true;
        break;
    case DateTimeFormat::FieldTypeMinute:
        m_hasMinute = true;
        break;
    case DateTimeFormat::FieldTypeSecond:
        m_hasSecond = true;
        break;
    default:
        break;
    }
}
