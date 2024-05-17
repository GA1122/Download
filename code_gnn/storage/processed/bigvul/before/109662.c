void Document::enqueuePageshowEvent(PageshowEventPersistence persisted)
{
    dispatchWindowEvent(PageTransitionEvent::create(EventTypeNames::pageshow, persisted), this);
}
