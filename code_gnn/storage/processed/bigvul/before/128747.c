void ReadableStream::callPullIfNeeded()
{
    if (m_isPulling || m_isDraining || !m_isStarted || m_state == Closed || m_state == Errored)
        return;

    bool shouldApplyBackpressure = this->shouldApplyBackpressure();
    if (m_state == Errored) {
        return;
    }

    if (!hasPendingReads() && shouldApplyBackpressure) {
        return;
    }
    m_isPulling = true;
    m_source->pullSource();
}
