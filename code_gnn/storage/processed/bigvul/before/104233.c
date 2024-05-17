RTCPeerConnection::RTCPeerConnection(ScriptExecutionContext* context, PassRefPtr<RTCConfiguration> configuration, PassRefPtr<MediaConstraints> constraints, ExceptionCode& ec)
    : ActiveDOMObject(context, this)
    , m_readyState(ReadyStateNew)
    , m_iceState(IceStateClosed)
    , m_localStreams(MediaStreamList::create())
    , m_remoteStreams(MediaStreamList::create())
{
    m_peerHandler = RTCPeerConnectionHandler::create(this);
    if (!m_peerHandler || !m_peerHandler->initialize(configuration, constraints))
        ec = NOT_SUPPORTED_ERR;
}
