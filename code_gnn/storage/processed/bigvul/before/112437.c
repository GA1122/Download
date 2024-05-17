void Document::didAddWheelEventHandler()
{
    ++m_wheelEventHandlerCount;
    Frame* mainFrame = page() ? page()->mainFrame() : 0;
    if (mainFrame)
        mainFrame->notifyChromeClientWheelEventHandlerCountChanged();

    wheelEventHandlerCountChanged(this);
}
