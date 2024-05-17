void RenderWidgetHostImpl::Paste() {
  Send(new ViewMsg_Paste(GetRoutingID()));
  RecordAction(UserMetricsAction("Paste"));
}
