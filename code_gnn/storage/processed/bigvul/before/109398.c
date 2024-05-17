void InspectorResourceAgent::willSendWebSocketHandshakeRequest(Document*, unsigned long identifier, const WebSocketHandshakeRequest& request)
{
    RefPtr<TypeBuilder::Network::WebSocketRequest> requestObject = TypeBuilder::Network::WebSocketRequest::create()
        .setHeaders(buildObjectForHeaders(request.headerFields()));
    m_frontend->webSocketWillSendHandshakeRequest(IdentifiersFactory::requestId(identifier), currentTime(), requestObject);
}
