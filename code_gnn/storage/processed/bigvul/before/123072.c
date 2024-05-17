void RenderWidgetHostImpl::Undo() {
  Send(new ViewMsg_Undo(GetRoutingID()));
  RecordAction(UserMetricsAction("Undo"));
}
