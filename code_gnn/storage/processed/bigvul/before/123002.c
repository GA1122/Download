void RenderWidgetHostImpl::OnMsgFocus() {
  RecordAction(UserMetricsAction("BadMessageTerminate_RWH4"));
  GetProcess()->ReceivedBadMessage();
}
