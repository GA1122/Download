void RenderFrameHostImpl::Paste() {
  Send(new InputMsg_Paste(routing_id_));
  RecordAction(base::UserMetricsAction("Paste"));
}
