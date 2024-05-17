void RTCPeerConnectionHandlerChromium::didAddRemoteStream(const WebKit::WebMediaStreamDescriptor& webMediaStreamDescriptor)
{
    m_client->didAddRemoteStream(webMediaStreamDescriptor);
}
