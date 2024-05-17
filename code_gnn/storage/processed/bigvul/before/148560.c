void WebContentsImpl::SetHistoryOffsetAndLengthForView(
    RenderViewHost* render_view_host,
    int history_offset,
    int history_length) {
  render_view_host->Send(new PageMsg_SetHistoryOffsetAndLength(
      render_view_host->GetRoutingID(), history_offset, history_length));
}
