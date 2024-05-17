void RenderWidgetHostImpl::LostMouseLock() {
  Send(new ViewMsg_MouseLockLost(routing_id_));
}
