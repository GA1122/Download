void Document::dispatchVisibilityStateChangeEvent()
{
    dispatchEvent(Event::create(EventTypeNames::webkitvisibilitychange));
}
