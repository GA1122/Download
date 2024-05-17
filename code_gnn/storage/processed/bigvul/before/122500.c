void InspectorResourceAgent::didReceiveWebSocketFrame(unsigned long identifier, int opCode, bool masked, const char* payload, size_t payloadLength)
{
    RefPtr<TypeBuilder::Network::WebSocketFrame> frameObject = TypeBuilder::Network::WebSocketFrame::create()
        .setOpcode(opCode)
        .setMask(masked)
        .setPayloadData(String(payload, payloadLength));
    m_frontend->webSocketFrameReceived(IdentifiersFactory::requestId(identifier), currentTime(), frameObject);
}
