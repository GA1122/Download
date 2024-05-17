void RenderViewHostImpl::ExitFullscreen() {
  RejectMouseLockOrUnlockIfNecessary();
  WasResized();
}
