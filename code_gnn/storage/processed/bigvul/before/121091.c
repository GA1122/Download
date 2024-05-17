void BaseMultipleFieldsDateAndTimeInputType::forwardEvent(Event* event)
{
    if (m_spinButtonElement) {
        m_spinButtonElement->forwardEvent(event);
        if (event->defaultHandled())
            return;
    }
        
    if (m_dateTimeEditElement)
        m_dateTimeEditElement->defaultEventHandler(event);
}
