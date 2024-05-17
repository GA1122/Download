void RenderWidgetHostImpl::Delete() {
  Send(new ViewMsg_Delete(GetRoutingID()));
  RecordAction(UserMetricsAction("DeleteSelection"));
}
