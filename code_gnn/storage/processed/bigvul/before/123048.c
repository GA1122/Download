void RenderWidgetHostImpl::SelectAll() {
  Send(new ViewMsg_SelectAll(GetRoutingID()));
  RecordAction(UserMetricsAction("SelectAll"));
}
