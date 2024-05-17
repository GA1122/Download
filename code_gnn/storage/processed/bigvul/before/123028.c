void RenderWidgetHostImpl::PasteAndMatchStyle() {
  Send(new ViewMsg_PasteAndMatchStyle(GetRoutingID()));
  RecordAction(UserMetricsAction("PasteAndMatchStyle"));
}
