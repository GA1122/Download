void BrowserPluginGuest::SetCursor(const WebCursor& cursor) {
  SendMessageToEmbedder(new BrowserPluginMsg_SetCursor(embedder_routing_id(),
                                                       instance_id(),
                                                       cursor));
}
