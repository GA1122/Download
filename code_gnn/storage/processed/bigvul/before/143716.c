void RenderWidgetHostImpl::ShutdownAndDestroyWidget(bool also_delete) {
  RejectMouseLockOrUnlockIfNecessary();

  if (process_->HasConnection()) {
    bool rv = Send(new ViewMsg_Close(routing_id_));
    DCHECK(rv);
  }

  Destroy(also_delete);
}
