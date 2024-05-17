void Document::setReadyState(ReadyState readyState)
{
    if (readyState == m_readyState)
        return;

    switch (readyState) {
    case Loading:
        if (!m_documentTiming.domLoading) {
            m_documentTiming.domLoading = monotonicallyIncreasingTime();
            if (RuntimeEnabledFeatures::webAnimationsEnabled())
                m_timeline->setZeroTimeAsPerfTime(m_documentTiming.domLoading);
        }
        break;
    case Interactive:
        if (!m_documentTiming.domInteractive)
            m_documentTiming.domInteractive = monotonicallyIncreasingTime();
        break;
    case Complete:
        if (!m_documentTiming.domComplete)
            m_documentTiming.domComplete = monotonicallyIncreasingTime();
        break;
    }

    m_readyState = readyState;
    dispatchEvent(Event::create(EventTypeNames::readystatechange));
}
