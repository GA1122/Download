void RenderWidgetHostImpl::DisableResizeAckCheckForTesting() {
  g_check_for_pending_resize_ack = false;
}
