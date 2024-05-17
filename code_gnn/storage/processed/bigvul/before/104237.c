void RTCPeerConnection::changeReadyState(ReadyState readyState)
{
    if (readyState == m_readyState || m_readyState == ReadyStateClosed)
        return;

    m_readyState = readyState;

    switch (m_readyState) {
    case ReadyStateOpening:
        break;
    case ReadyStateActive:
        dispatchEvent(Event::create(eventNames().openEvent, false, false));
        break;
    case ReadyStateClosing:
    case ReadyStateClosed:
        break;
    case ReadyStateNew:
        ASSERT_NOT_REACHED();
        break;
    }

    dispatchEvent(Event::create(eventNames().statechangeEvent, false, false));
}
