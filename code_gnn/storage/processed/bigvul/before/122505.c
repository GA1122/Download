void InspectorResourceAgent::frameClearedScheduledNavigation(LocalFrame* frame)
{
    m_frameNavigationInitiatorMap.remove(m_pageAgent->frameId(frame));
}
