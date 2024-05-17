bool BaseMultipleFieldsDateAndTimeInputType::hasBadInput() const
{
    return element()->value().isEmpty() && m_dateTimeEditElement && m_dateTimeEditElement->anyEditableFieldsHaveValues();
}
