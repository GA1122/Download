void RenderFrameHostImpl::Copy() {
  Send(new InputMsg_Copy(routing_id_));
  RecordAction(base::UserMetricsAction("Copy"));
}
