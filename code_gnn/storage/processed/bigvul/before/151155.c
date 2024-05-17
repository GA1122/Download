void InspectorNetworkAgent::DidCloseWebSocket(Document*,
                                              unsigned long identifier) {
  GetFrontend()->webSocketClosed(IdentifiersFactory::RequestId(identifier),
                                 MonotonicallyIncreasingTime());
}
