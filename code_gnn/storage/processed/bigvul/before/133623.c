void WebContentsImpl::RouteMessageEvent(
    RenderViewHost* rvh,
    const ViewMsg_PostMessage_Params& params) {
  if (!rvh->GetSiteInstance()->IsRelatedSiteInstance(GetSiteInstance()) &&
      !GetBrowserPluginGuest() && !GetBrowserPluginEmbedder())
    return;

  ViewMsg_PostMessage_Params new_params(params);

  if (!params.message_port_ids.empty()) {
    MessagePortMessageFilter* message_port_message_filter =
        static_cast<RenderProcessHostImpl*>(GetRenderProcessHost())
            ->message_port_message_filter();
    std::vector<int> new_routing_ids(params.message_port_ids.size());
    for (size_t i = 0; i < params.message_port_ids.size(); ++i) {
      new_routing_ids[i] = message_port_message_filter->GetNextRoutingID();
      MessagePortService::GetInstance()->UpdateMessagePort(
          params.message_port_ids[i],
          message_port_message_filter,
          new_routing_ids[i]);
    }
    new_params.new_routing_ids = new_routing_ids;
  }

  if (new_params.source_routing_id != MSG_ROUTING_NONE) {
    WebContentsImpl* source_contents = NULL;
    RenderViewHostImpl* source_rvh = RenderViewHostImpl::FromID(
        rvh->GetProcess()->GetID(), params.source_routing_id);
    if (source_rvh) {
      source_contents = static_cast<WebContentsImpl*>(
          source_rvh->GetDelegate()->GetAsWebContents());
    }

    if (source_contents) {
      if (GetBrowserPluginGuest()) {
        new_params.source_routing_id =
            source_contents->CreateSwappedOutRenderView(GetSiteInstance());
      } else {
        new_params.source_routing_id =
            source_contents->CreateOpenerRenderViews(GetSiteInstance());
      }
    } else {
      new_params.source_routing_id = MSG_ROUTING_NONE;
    }
  }

  Send(new ViewMsg_PostMessageEvent(GetRoutingID(), new_params));
}
