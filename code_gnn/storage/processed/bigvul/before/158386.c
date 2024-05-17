void RenderWidgetHostImpl::ViewDestroyed() {
  CancelKeyboardLock();
  RejectMouseLockOrUnlockIfNecessary();

  SetView(nullptr);
}
