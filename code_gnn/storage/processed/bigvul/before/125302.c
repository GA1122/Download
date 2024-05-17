void DocumentTimeline::eventDispatchTimerFired(Timer<DocumentTimeline>*)
{
    dispatchEvents();
}
