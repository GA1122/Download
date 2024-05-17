bool WebPagePrivate::handleWheelEvent(PlatformWheelEvent& wheelEvent)
{
    return m_mainFrame->eventHandler()->handleWheelEvent(wheelEvent);
}
