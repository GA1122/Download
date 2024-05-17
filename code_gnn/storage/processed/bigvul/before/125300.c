void DocumentTimeline::dispatchEvents()
{
    Vector<EventToDispatch> events = m_events;
    m_events.clear();
    for (size_t i = 0; i < events.size(); i++)
        events[i].target->dispatchEvent(events[i].event.release());
}
