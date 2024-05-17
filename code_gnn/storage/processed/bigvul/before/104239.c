PassRefPtr<RTCPeerConnection> RTCPeerConnection::create(ScriptExecutionContext* context, const Dictionary& rtcConfiguration, const Dictionary& mediaConstraints, ExceptionCode& ec)
{
    RefPtr<RTCConfiguration> configuration = parseConfiguration(rtcConfiguration, ec);
    if (ec)
        return 0;

    RefPtr<MediaConstraints> constraints = MediaConstraintsImpl::create(mediaConstraints, ec);
    if (ec)
        return 0;

    RefPtr<RTCPeerConnection> peerConnection = adoptRef(new RTCPeerConnection(context, configuration.release(), constraints.release(), ec));
    peerConnection->suspendIfNeeded();
    if (ec)
        return 0;

    return peerConnection.release();
}
