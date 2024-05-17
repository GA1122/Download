void RTCPeerConnectionHandlerChromium::didChangeICEState(WebKit::WebRTCPeerConnectionHandlerClient::ICEState state)
{
    m_client->didChangeIceState(static_cast<RTCPeerConnectionHandlerClient::IceState>(state));
}
