void* HTMLInputElement::preDispatchEventHandler(Event* event)
{
    if (event->type() == eventNames().textInputEvent && m_inputType->shouldSubmitImplicitly(event)) {
        event->stopPropagation();
        return 0;
    }
    if (event->type() != eventNames().clickEvent)
        return 0;
    if (!event->isMouseEvent() || static_cast<MouseEvent*>(event)->button() != LeftButton)
        return 0;
    return m_inputType->willDispatchClick().leakPtr();
}
