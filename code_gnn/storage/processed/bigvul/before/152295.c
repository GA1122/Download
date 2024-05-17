void RenderFrameImpl::DidStopLoading() {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::didStopLoading",
               "id", routing_id_);

  history_subframe_unique_names_.clear();

  SendUpdateFaviconURL();

  Send(new FrameHostMsg_DidStopLoading(routing_id_));
}
