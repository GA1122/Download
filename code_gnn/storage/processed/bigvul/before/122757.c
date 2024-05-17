void BrowserPluginGuest::LoadRedirect(
    const GURL& old_url,
    const GURL& new_url,
    bool is_top_level) {
  SendMessageToEmbedder(
      new BrowserPluginMsg_LoadRedirect(embedder_routing_id(),
                                        instance_id(),
                                        old_url,
                                        new_url,
                                        is_top_level));
}
