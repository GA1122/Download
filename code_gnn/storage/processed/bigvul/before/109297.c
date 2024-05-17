void InspectorPageAgent::frameScheduledNavigation(Frame* frame, double delay)
{
    m_frontend->frameScheduledNavigation(frameId(frame), delay);
}
