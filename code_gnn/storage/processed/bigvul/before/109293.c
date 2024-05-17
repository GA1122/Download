void InspectorPageAgent::frameClearedScheduledNavigation(Frame* frame)
{
    m_frontend->frameClearedScheduledNavigation(frameId(frame));
}
