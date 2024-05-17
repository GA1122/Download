void Document::enqueueUniqueAnimationFrameEvent(Event* event)
{
    ensureScriptedAnimationController().enqueuePerFrameEvent(event);
}
