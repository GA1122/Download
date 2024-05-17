void RenderWidgetHostImpl::LostCapture() {
  Send(new ViewMsg_MouseCaptureLost(routing_id_));
}
