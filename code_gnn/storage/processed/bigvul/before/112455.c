void Document::enqueuePageshowEvent(PageshowEventPersistence persisted)
{
    dispatchWindowEvent(PageTransitionEvent::create(eventNames().pageshowEvent, persisted), this);
}
