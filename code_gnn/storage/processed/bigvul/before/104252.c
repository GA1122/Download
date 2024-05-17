void RTCPeerConnection::removeStream(MediaStream* stream, ExceptionCode& ec)
{
    if (m_readyState == ReadyStateClosed) {
        ec = INVALID_STATE_ERR;
        return;
    }

    if (!stream) {
        ec = TYPE_MISMATCH_ERR;
        return;
    }

    if (!m_localStreams->contains(stream))
        return;

    m_localStreams->remove(stream);

    m_peerHandler->removeStream(stream->descriptor());
}
