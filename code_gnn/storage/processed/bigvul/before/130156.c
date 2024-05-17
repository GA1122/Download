void RenderFrameHostImpl::Cut() {
  Send(new InputMsg_Cut(routing_id_));
  RecordAction(base::UserMetricsAction("Cut"));
}
