void RTCPeerConnection::changeIceState(IceState iceState)
{
    if (iceState == m_iceState || m_readyState == ReadyStateClosed)
        return;

    m_iceState = iceState;
    dispatchEvent(Event::create(eventNames().icechangeEvent, false, false));
}
