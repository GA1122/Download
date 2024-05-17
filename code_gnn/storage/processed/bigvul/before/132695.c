LayoutTestContentRendererClient::OverrideCreateMIDIAccessor(
    WebMIDIAccessorClient* client) {
  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  return interfaces->CreateMIDIAccessor(client);
}
