PassRefPtr<Event> Document::createEvent(const String& eventType, ExceptionState& es)
{
    RefPtr<Event> event = EventFactory::create(eventType);
    if (event)
        return event.release();

    es.throwUninformativeAndGenericDOMException(NotSupportedError);
    return 0;
}
