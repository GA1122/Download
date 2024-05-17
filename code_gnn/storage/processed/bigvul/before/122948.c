void RenderWidgetHostImpl::Cut() {
  Send(new ViewMsg_Cut(GetRoutingID()));
  RecordAction(UserMetricsAction("Cut"));
}
