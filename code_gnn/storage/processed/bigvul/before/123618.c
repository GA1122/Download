void InspectorPageAgent::loadEventFired(Frame* frame)
{
    if (!frame->isMainFrame())
        return;
    m_frontend->loadEventFired(currentTime());
}
