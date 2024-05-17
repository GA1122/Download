void RenderWidgetHostImpl::ResetSizeAndRepaintPendingFlags() {
  resize_ack_pending_ = false;
  repaint_ack_pending_ = false;
  in_flight_size_.SetSize(0, 0);
}
