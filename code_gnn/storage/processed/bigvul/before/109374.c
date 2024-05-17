void InspectorResourceAgent::didSendWebSocketFrame(unsigned long identifier, const WebSocketFrame& frame)
{
    RefPtr<TypeBuilder::Network::WebSocketFrame> frameObject = TypeBuilder::Network::WebSocketFrame::create()
        .setOpcode(frame.opCode)
        .setMask(frame.masked)
        .setPayloadData(String(frame.payload, frame.payloadLength));
    m_frontend->webSocketFrameSent(IdentifiersFactory::requestId(identifier), currentTime(), frameObject);
}