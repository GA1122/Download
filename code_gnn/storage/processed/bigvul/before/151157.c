void InspectorNetworkAgent::DidCreateWebSocket(Document* document,
                                               unsigned long identifier,
                                               const KURL& request_url,
                                               const String&) {
  std::unique_ptr<v8_inspector::protocol::Runtime::API::StackTrace>
      current_stack_trace =
          SourceLocation::Capture(document)->BuildInspectorObject();
  if (!current_stack_trace) {
    GetFrontend()->webSocketCreated(
        IdentifiersFactory::RequestId(identifier),
        UrlWithoutFragment(request_url).GetString());
    return;
  }

  std::unique_ptr<protocol::Network::Initiator> initiator_object =
      protocol::Network::Initiator::create()
          .setType(protocol::Network::Initiator::TypeEnum::Script)
          .build();
  initiator_object->setStack(std::move(current_stack_trace));
  GetFrontend()->webSocketCreated(IdentifiersFactory::RequestId(identifier),
                                  UrlWithoutFragment(request_url).GetString(),
                                  std::move(initiator_object));
}
