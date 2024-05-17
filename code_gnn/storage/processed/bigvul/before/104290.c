void RTCPeerConnectionHandlerChromium::didChangeReadyState(WebKit::WebRTCPeerConnectionHandlerClient::ReadyState state)
{
    m_client->didChangeReadyState(static_cast<RTCPeerConnectionHandlerClient::ReadyState>(state));
}
