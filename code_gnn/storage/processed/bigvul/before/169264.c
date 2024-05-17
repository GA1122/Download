bool IsResizeComplete(RenderWidgetHostImpl* widget_host) {
  return !widget_host->resize_ack_pending_for_testing();
}
