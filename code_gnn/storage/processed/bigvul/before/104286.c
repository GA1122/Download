PassOwnPtr<RTCPeerConnectionHandler> RTCPeerConnectionHandler::create(RTCPeerConnectionHandlerClient* client)
{
    return adoptPtr(new RTCPeerConnectionHandlerChromium(client));
}
