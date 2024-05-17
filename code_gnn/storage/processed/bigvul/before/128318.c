void FrameView::scheduleUpdateWidgetsIfNecessary()
{
    ASSERT(!isInPerformLayout());
    if (m_updateWidgetsTimer.isActive() || m_widgetUpdateSet.isEmpty())
        return;
    m_updateWidgetsTimer.startOneShot(0, FROM_HERE);
}
