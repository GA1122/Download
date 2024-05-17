void BrowserPluginGuest::DidFailProvisionalLoad(
    int64 frame_id,
    bool is_main_frame,
    const GURL& validated_url,
    int error_code,
    const string16& error_description,
    RenderViewHost* render_view_host) {
  std::string error_type;
  RemoveChars(net::ErrorToString(error_code), "net::", &error_type);
  SendMessageToEmbedder(
      new BrowserPluginMsg_LoadAbort(embedder_routing_id(),
                                     instance_id(),
                                     validated_url,
                                     is_main_frame,
                                     error_type));
}
