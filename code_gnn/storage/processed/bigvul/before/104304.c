void MockWebRTCPeerConnectionHandler::removeStream(const WebMediaStreamDescriptor& stream)
{
    m_client->didRemoveRemoteStream(stream);
}
