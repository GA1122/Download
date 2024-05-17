void RenderWidgetHostImpl::ShutdownAndDestroyWidget(bool also_delete) {
  CancelKeyboardLock();
  RejectMouseLockOrUnlockIfNecessary();

  if (process_->IsInitializedAndNotDead()) {
    if (owner_delegate_) {
      owner_delegate_->RenderWidgetDidShutdown();
    } else {
      bool rv = Send(new ViewMsg_Close(routing_id_));
      DCHECK(rv);
    }
  }

  Destroy(also_delete);
}
