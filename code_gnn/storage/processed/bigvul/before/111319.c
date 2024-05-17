void WebPagePrivate::scheduleRootLayerCommit()
{
    if (!(m_frameLayers && m_frameLayers->hasLayer()) && !m_overlayLayer)
        return;

    m_needsCommit = true;
    if (!m_rootLayerCommitTimer->isActive()) {
#if DEBUG_AC_COMMIT
        BBLOG(Platform::LogLevelCritical, "%s: m_rootLayerCommitTimer->isActive() = %d", WTF_PRETTY_FUNCTION, m_rootLayerCommitTimer->isActive());
#endif
        m_rootLayerCommitTimer->startOneShot(0);
    }
}
