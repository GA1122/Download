void Document::enqueueWindowEvent(PassRefPtr<Event> event)
{
    event->setTarget(domWindow());
    m_eventQueue->enqueueEvent(event);
}
