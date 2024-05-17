void Document::dispatchVisibilityStateChangeEvent()
{
    dispatchEvent(Event::create(eventNames().webkitvisibilitychangeEvent, false, false));
}
