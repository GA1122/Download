void RTCPeerConnection::addIceCandidate(RTCIceCandidate* iceCandidate, ExceptionCode& ec)
{
    if (m_readyState == ReadyStateClosing || m_readyState == ReadyStateClosed) {
        ec = INVALID_STATE_ERR;
        return;
    }

    if (!iceCandidate) {
        ec = TYPE_MISMATCH_ERR;
        return;
    }

    bool valid = m_peerHandler->addIceCandidate(iceCandidate->descriptor());
    if (!valid)
        ec = SYNTAX_ERR;
}
