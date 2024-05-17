void Document::enqueueAnimationFrameEvent(Event* event)
{
    ensureScriptedAnimationController().enqueueEvent(event);
}
