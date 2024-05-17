void BrowserPluginGuest::UpdateRectACK(
    int message_id,
    const BrowserPluginHostMsg_AutoSize_Params& auto_size_params,
    const BrowserPluginHostMsg_ResizeGuest_Params& resize_guest_params) {
  RenderViewHost* render_view_host = pending_updates_.Lookup(message_id);
  if (render_view_host) {
    pending_updates_.Remove(message_id);
    render_view_host->Send(
        new ViewMsg_UpdateRect_ACK(render_view_host->GetRoutingID()));
  }
  SetSize(auto_size_params, resize_guest_params);
}
