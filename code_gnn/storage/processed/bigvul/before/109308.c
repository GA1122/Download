void InspectorPageAgent::loadEventFired(Frame* frame)
{
    if (frame->page()->mainFrame() != frame)
        return;
    m_frontend->loadEventFired(currentTime());
}
