void RTCPeerConnection::addStream(PassRefPtr<MediaStream> prpStream, const Dictionary& mediaConstraints, ExceptionCode& ec)
{
    if (m_readyState == ReadyStateClosing || m_readyState == ReadyStateClosed) {
        ec = INVALID_STATE_ERR;
        return;
    }

    RefPtr<MediaStream> stream = prpStream;
    if (!stream) {
        ec =  TYPE_MISMATCH_ERR;
        return;
    }

    RefPtr<MediaConstraints> constraints = MediaConstraintsImpl::create(mediaConstraints, ec);
    if (ec)
        return;

    if (m_localStreams->contains(stream.get()))
        return;

    m_localStreams->append(stream);

    bool valid = m_peerHandler->addStream(stream->descriptor(), constraints);
    if (!valid)
        ec = SYNTAX_ERR;
}
