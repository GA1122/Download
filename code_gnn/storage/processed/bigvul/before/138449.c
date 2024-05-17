void Document::setReadyState(ReadyState readyState)
{
    if (readyState == m_readyState)
        return;

    switch (readyState) {
    case Loading:
        if (!m_documentTiming.domLoading()) {
            m_documentTiming.setDomLoading(monotonicallyIncreasingTime());
        }
        break;
    case Interactive:
        if (!m_documentTiming.domInteractive())
            m_documentTiming.setDomInteractive(monotonicallyIncreasingTime());
        break;
    case Complete:
        if (!m_documentTiming.domComplete())
            m_documentTiming.setDomComplete(monotonicallyIncreasingTime());
        break;
    }

    m_readyState = readyState;
    dispatchEvent(Event::create(EventTypeNames::readystatechange));
}
