void Document::setVisualUpdatesAllowed(ReadyState readyState)
{
    ASSERT(settings() && settings()->suppressesIncrementalRendering());
    switch (readyState) {
    case Loading:
        ASSERT(!m_visualUpdatesSuppressionTimer.isActive());
        ASSERT(m_visualUpdatesAllowed);
        m_visualUpdatesSuppressionTimer.startOneShot(settings()->incrementalRenderingSuppressionTimeoutInSeconds());
        setVisualUpdatesAllowed(false);
        break;
    case Interactive:
        ASSERT(m_visualUpdatesSuppressionTimer.isActive() || m_visualUpdatesAllowed);
        break;
    case Complete:
        if (m_visualUpdatesSuppressionTimer.isActive()) {
            ASSERT(!m_visualUpdatesAllowed);
            m_visualUpdatesSuppressionTimer.stop();
            setVisualUpdatesAllowed(true);
        } else
            ASSERT(m_visualUpdatesAllowed);
        break;
    }
}
