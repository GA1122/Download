void Document::didRemoveWheelEventHandler()
{
    ASSERT(m_wheelEventHandlerCount > 0);
    --m_wheelEventHandlerCount;
    Frame* mainFrame = page() ? page()->mainFrame() : 0;
    if (mainFrame)
        mainFrame->notifyChromeClientWheelEventHandlerCountChanged();

    wheelEventHandlerCountChanged(this);
}
