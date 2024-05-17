void RenderWidgetHostImpl::Shutdown() {
  RejectMouseLockOrUnlockIfNecessary();

  if (process_->HasConnection()) {
    bool rv = Send(new ViewMsg_Close(routing_id_));
    DCHECK(rv);
  }

  Destroy();
}
