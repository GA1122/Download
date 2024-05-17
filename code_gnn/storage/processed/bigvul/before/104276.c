PassOwnPtr<RTCPeerConnectionHandler> RTCPeerConnectionHandler::create(RTCPeerConnectionHandlerClient* client)
{
     return adoptPtr(new RTCPeerConnectionHandlerDummy(client));
 }
