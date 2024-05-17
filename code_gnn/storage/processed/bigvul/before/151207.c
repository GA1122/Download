void InspectorNetworkAgent::WillSendWebSocketHandshakeRequest(
    Document*,
    unsigned long identifier,
    const WebSocketHandshakeRequest* request) {
  DCHECK(request);
  std::unique_ptr<protocol::Network::WebSocketRequest> request_object =
      protocol::Network::WebSocketRequest::create()
          .setHeaders(BuildObjectForHeaders(request->HeaderFields()))
          .build();
  GetFrontend()->webSocketWillSendHandshakeRequest(
      IdentifiersFactory::RequestId(identifier), MonotonicallyIncreasingTime(),
      CurrentTime(), std::move(request_object));
}
