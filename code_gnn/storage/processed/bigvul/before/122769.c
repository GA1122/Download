void BrowserPluginGuest::SetIsAcceptingTouchEvents(bool accept) {
  SendMessageToEmbedder(
      new BrowserPluginMsg_ShouldAcceptTouchEvents(embedder_routing_id(),
                                                   instance_id(),
                                                   accept));
}
