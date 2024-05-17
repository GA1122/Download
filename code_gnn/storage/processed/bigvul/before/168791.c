void RenderWidgetHostImpl::ViewDestroyed() {
  RejectMouseLockOrUnlockIfNecessary();

  SetView(nullptr);
}
