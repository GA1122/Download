void BrowserPluginGuest::DidStartProvisionalLoadForFrame(
    int64 frame_id,
    int64 parent_frame_id,
    bool is_main_frame,
    const GURL& validated_url,
    bool is_error_page,
    RenderViewHost* render_view_host) {
  SendMessageToEmbedder(
      new BrowserPluginMsg_LoadStart(embedder_routing_id(),
                                     instance_id(),
                                     validated_url,
                                     is_main_frame));
}
