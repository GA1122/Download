void RenderWidgetHostImpl::OnMsgBlur() {
  RecordAction(UserMetricsAction("BadMessageTerminate_RWH5"));
  GetProcess()->ReceivedBadMessage();
}
