void DocumentTimeline::dispatchEventsAsync()
{
    if (m_events.isEmpty() || m_eventDistpachTimer.isActive())
        return;
    m_eventDistpachTimer.startOneShot(0);
}
