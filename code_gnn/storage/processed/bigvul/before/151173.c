void InspectorNetworkAgent::DidReceiveWebSocketHandshakeResponse(
    Document*,
    unsigned long identifier,
    const WebSocketHandshakeRequest* request,
    const WebSocketHandshakeResponse* response) {
  DCHECK(response);
  std::unique_ptr<protocol::Network::WebSocketResponse> response_object =
      protocol::Network::WebSocketResponse::create()
          .setStatus(response->StatusCode())
          .setStatusText(response->StatusText())
          .setHeaders(BuildObjectForHeaders(response->HeaderFields()))
          .build();

  if (!response->HeadersText().IsEmpty())
    response_object->setHeadersText(response->HeadersText());
  if (request) {
    response_object->setRequestHeaders(
        BuildObjectForHeaders(request->HeaderFields()));
    if (!request->HeadersText().IsEmpty())
      response_object->setRequestHeadersText(request->HeadersText());
  }
  GetFrontend()->webSocketHandshakeResponseReceived(
      IdentifiersFactory::RequestId(identifier), MonotonicallyIncreasingTime(),
      std::move(response_object));
}
