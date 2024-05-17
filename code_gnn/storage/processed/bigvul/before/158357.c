void RenderWidgetHostImpl::SendMouseLockLost() {
  Send(new ViewMsg_MouseLockLost(routing_id_));
}
