void Document::enqueueVisualViewportChangedEvent()
{
    Event* event = Event::create(EventTypeNames::visualviewportchanged);
    event->setTarget(domWindow());
    ensureScriptedAnimationController().enqueuePerFrameEvent(event);
}
