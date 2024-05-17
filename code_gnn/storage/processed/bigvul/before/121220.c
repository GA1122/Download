void HTMLInputElement::postDispatchEventHandler(Event* event, void* dataFromPreDispatch)
{
    OwnPtr<ClickHandlingState> state = adoptPtr(static_cast<ClickHandlingState*>(dataFromPreDispatch));
    if (!state)
        return;
    m_inputType->didDispatchClick(event, *state);
}