PassRefPtrWillBeRawPtr<Event> Document::createEvent(const String& eventType, ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<Event> event = nullptr;
    for (const auto& factory : eventFactories()) {
        event = factory->create(eventType);
        if (event)
            return event.release();
    }
    exceptionState.throwDOMException(NotSupportedError, "The provided event type ('" + eventType + "') is invalid.");
    return nullptr;
}
