void Document::enqueueResizeEvent()
{
    Event* event = Event::create(EventTypeNames::resize);
    event->setTarget(domWindow());
    ensureScriptedAnimationController().enqueuePerFrameEvent(event);
}
