void Document::enqueueAnimationFrameEvent(PassRefPtrWillBeRawPtr<Event> event)
{
    ensureScriptedAnimationController().enqueueEvent(event);
}
