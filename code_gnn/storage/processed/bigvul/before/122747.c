void BrowserPluginGuest::DidStopLoading(RenderViewHost* render_view_host) {
  SendMessageToEmbedder(new BrowserPluginMsg_LoadStop(embedder_routing_id(),
                                                      instance_id()));
}
