void FrameView::updateWidgetsTimerFired(Timer<FrameView>*)
{
    ASSERT(!isInPerformLayout());
    RefPtr<FrameView> protect(this);
    m_updateWidgetsTimer.stop();
    for (unsigned i = 0; i < maxUpdateWidgetsIterations; ++i) {
        if (updateWidgets())
            return;
    }
}
