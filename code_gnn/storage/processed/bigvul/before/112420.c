PassRefPtr<Event> Document::createEvent(const String& eventType, ExceptionCode& ec)
{
    RefPtr<Event> event = EventFactory::create(eventType);
    if (event)
        return event.release();

    ec = NOT_SUPPORTED_ERR;
    return 0;
}
