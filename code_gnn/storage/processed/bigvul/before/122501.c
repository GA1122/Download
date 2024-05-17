void InspectorResourceAgent::didReceiveWebSocketHandshakeResponse(Document*, unsigned long identifier, const WebSocketHandshakeRequest* request, const WebSocketHandshakeResponse* response)
{
    ASSERT(response);
    RefPtr<TypeBuilder::Network::WebSocketResponse> responseObject = TypeBuilder::Network::WebSocketResponse::create()
        .setStatus(response->statusCode())
        .setStatusText(response->statusText())
        .setHeaders(buildObjectForHeaders(response->headerFields()));

    if (!response->headersText().isEmpty())
        responseObject->setHeadersText(response->headersText());
    if (request) {
        responseObject->setRequestHeaders(buildObjectForHeaders(request->headerFields()));
        if (!request->headersText().isEmpty())
            responseObject->setRequestHeadersText(request->headersText());
    }
    m_frontend->webSocketHandshakeResponseReceived(IdentifiersFactory::requestId(identifier), currentTime(), responseObject);
}
