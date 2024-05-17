void RenderWidgetHostImpl::DisableResizeAckCheckForTesting() {
  g_check_for_pending_visual_properties_ack = false;
}
