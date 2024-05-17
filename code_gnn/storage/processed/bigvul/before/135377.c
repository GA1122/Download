void Document::setReadyState(ReadyState readyState)
{
    if (readyState == m_readyState)
        return;

    switch (readyState) {
    case Loading:
        if (!m_documentTiming.domLoading()) {
            m_documentTiming.markDomLoading();
        }
        break;
    case Interactive:
        if (!m_documentTiming.domInteractive())
            m_documentTiming.markDomInteractive();
        break;
    case Complete:
        if (!m_documentTiming.domComplete())
            m_documentTiming.markDomComplete();
        break;
    }

    m_readyState = readyState;
    dispatchEvent(Event::create(EventTypeNames::readystatechange));
}
