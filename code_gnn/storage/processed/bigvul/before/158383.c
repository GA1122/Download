void RenderWidgetHostImpl::SynchronizeVisualPropertiesIgnoringPendingAck() {
  visual_properties_ack_pending_ = false;
  SynchronizeVisualProperties();
}
