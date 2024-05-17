void BrowserPluginGuest::DidCommitProvisionalLoadForFrame(
    int64 frame_id,
    bool is_main_frame,
    const GURL& url,
    PageTransition transition_type,
    RenderViewHost* render_view_host) {
  BrowserPluginMsg_LoadCommit_Params params;
  params.url = url;
  params.is_top_level = is_main_frame;
  params.process_id = render_view_host->GetProcess()->GetID();
  params.current_entry_index =
      web_contents()->GetController().GetCurrentEntryIndex();
  params.entry_count =
      web_contents()->GetController().GetEntryCount();
  SendMessageToEmbedder(
      new BrowserPluginMsg_LoadCommit(embedder_routing_id(),
                                      instance_id(),
                                      params));
  RecordAction(UserMetricsAction("BrowserPlugin.Guest.DidNavigate"));
}
