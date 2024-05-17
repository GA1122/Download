void RTCPeerConnection::stop()
{
    m_iceState = IceStateClosed;
    m_readyState = ReadyStateClosed;

    if (m_peerHandler) {
        m_peerHandler->stop();
        m_peerHandler.clear();
    }
}
