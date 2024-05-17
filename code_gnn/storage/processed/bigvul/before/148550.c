void WebContentsImpl::SendPpapiBrokerPermissionResult(int process_id,
                                                      int ppb_broker_route_id,
                                                      bool result) {
  RenderProcessHost* rph = RenderProcessHost::FromID(process_id);
  if (rph) {
    rph->Send(
        new ViewMsg_PpapiBrokerPermissionResult(ppb_broker_route_id, result));
  }
}
