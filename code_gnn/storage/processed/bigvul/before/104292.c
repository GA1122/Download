void RTCPeerConnectionHandlerChromium::didRemoveRemoteStream(const WebKit::WebMediaStreamDescriptor& webMediaStreamDescriptor)
{
    m_client->didRemoveRemoteStream(webMediaStreamDescriptor);
}
