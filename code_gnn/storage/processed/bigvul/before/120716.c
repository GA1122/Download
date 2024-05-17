void BaseMultipleFieldsDateAndTimeInputType::forwardEvent(Event* event)
{
    if (SpinButtonElement* element = spinButtonElement()) {
        element->forwardEvent(event);
        if (event->defaultHandled())
            return;
    }
        
    if (DateTimeEditElement* edit = dateTimeEditElement())
        edit->defaultEventHandler(event);
}
