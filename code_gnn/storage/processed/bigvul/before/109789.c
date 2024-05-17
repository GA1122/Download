void Document::scheduleAnimationFrameEvent(PassRefPtr<Event> event)
{
    ensureScriptedAnimationController().scheduleEvent(event);
}
