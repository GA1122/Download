void InspectorResourceAgent::frameClearedScheduledNavigation(Frame* frame)
{
    m_frameNavigationInitiatorMap.remove(m_pageAgent->frameId(frame));
}
